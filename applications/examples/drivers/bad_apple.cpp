// --- GL Includes ---
#include "ciegl/ciegl.hpp"

// --- CSG Includes ---
#include "csg/trees.hpp"

// --- Utility Includes ---
#include "cieutils/commandline.hpp"
#include "cieutils/concepts.hpp"
#include "cieutils/macros.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <stdlib.h>
#include <chrono>
#include <filesystem>
#include <set>


namespace cie {


// -------------------------------------------------------------------------
// TYPE ALIASES
// -------------------------------------------------------------------------

using CoordinateType = float;
using ValueType      = bool;
using PointType      = csg::CSGTraits<2,CoordinateType>::point_type;

using Primitive      = csg::Cube<2,CoordinateType>;
using Cell           = csg::Cell<Primitive>;
using Node           = csg::SpaceTreeNode<Cell,ValueType>;
using NodePtr        = std::shared_ptr<Node>;

using Sampler        = csg::CartesianGridSampler<Primitive>;
using Splitter       = csg::MidPointSplitPolicy<Node::sample_point_iterator,Node::value_iterator>;


// -------------------------------------------------------------------------
// GLOBALS
// -------------------------------------------------------------------------

const std::filesystem::path SHADER_DIR = SOURCE_PATH / "libraries/ciegl/data/shaders/rectangleFrame";

const Size numberOfSamplePointsPerDimension = 12;
const Size depth = 8;


// -------------------------------------------------------------------------
// CLASS DECLARATION
// -------------------------------------------------------------------------

class BadAppleScene : public gl::Scene
{
public:
    BadAppleScene( utils::Logger& r_logger,
                   const std::string& r_name,
                   utils::CommandLineArguments& args );

    gl::CameraPtr getCamera();

    std::string& framePath();

    void updateTree();

private:
    void update_impl();

    template <concepts::Integer CT>
    ValueType sampleImage( CT x, CT y ) const;

    template <class CT>
    requires (!concepts::Integer<CT>)
    ValueType sampleImage( CT x, CT y ) const;

private:
    std::string          _framePath;
    NodePtr              _p_root;
    gl::Image            _frame;
    std::pair<Size,Size> _frameSize;
    mp::ThreadPool       _threadPool;

    gl::VertexBuffer::data_container_type _vertexData;
};


// -------------------------------------------------------------------------
// CLASS DEFINITION
// -------------------------------------------------------------------------

BadAppleScene::BadAppleScene( utils::Logger& r_logger,
                              const std::string& r_name,
                              utils::CommandLineArguments& args ) :
    gl::Scene( r_logger,
               r_name,
               gl::makeVertexShader<gl::GLFWVertexShader>( SHADER_DIR / "vertexShader.xml", SHADER_DIR / "vertexShader.glsl" ),
               gl::makeGeometryShader<gl::GLFWGeometryShader>( SHADER_DIR / "geometryShader.xml", SHADER_DIR / "geometryShader.glsl" ),
               gl::makeFragmentShader<gl::GLFWFragmentShader>( SHADER_DIR / "fragmentShader.xml", SHADER_DIR / "fragmentShader.glsl" ) )
{
    if ( args.arguments().empty() )
        throw std::runtime_error( "No frame specified!" );

    args.addDefaultKeywordArgument( "width", "160" );
    args.addDefaultKeywordArgument( "height", "120" );
    _frameSize.first = args.get<Size>( "width" );
    _frameSize.second = args.get<Size>( "height" );

    this->_framePath = args.arguments()[0];

    auto p_camera = this->makeCamera<gl::Camera<gl::OrthographicProjection>>();
    this->bindUniform( "transformation", p_camera->transformationMatrix() );

    this->_p_root = NodePtr( new Node(
        Node::sampler_ptr( new Sampler(numberOfSamplePointsPerDimension) ),
        Node::split_policy_ptr( new Splitter ),
        0,
        PointType {0.0, 0.0},
        1.0
    ) );

    this->_vertexData.resize( 4 * intPow(4, depth) );
}


gl::CameraPtr BadAppleScene::getCamera()
{
    return *this->_cameras.begin();
}


std::string& BadAppleScene::framePath()
{
    return this->_framePath;
}


void BadAppleScene::updateTree()
{
    this->_vertexData.resize( 4 * intPow(4, depth) );

    // Load next frame
    this->_frame.load( this->_framePath.c_str(), true );

    // Define target function and divide
    auto target = [this]( const PointType& r_point )
    { return this->sampleImage( r_point[0], r_point[1] ); };

    this->_p_root->clear();
    this->_p_root->divide(
        target
        ,depth
        //,this->_threadPool
    );
    this->_threadPool.barrier();

    // Fill vertex data
    Size dataIndex = 0;
    auto collectVertexData = [this, &dataIndex]( Node* p_node )
    {
        this->_vertexData[dataIndex++] = p_node->base()[0];
        this->_vertexData[dataIndex++] = p_node->base()[1];
        this->_vertexData[dataIndex++] = p_node->length();
        this->_vertexData[dataIndex++] = p_node->length();
        return true;
    };

    this->_p_root->visit( collectVertexData );
    this->_vertexData.resize( dataIndex );

    // Write vertex data to buffer
    this->_p_bufferManager->writeToBoundVertexBuffer( this->_vertexData );
}


void BadAppleScene::update_impl()
{
    glDrawArrays( GL_POINTS, 0, this->_vertexData.size() );
}


template <concepts::Integer CT>
ValueType BadAppleScene::sampleImage( CT x, CT y ) const
{
    CIE_OUT_OF_RANGE_CHECK( x < this->_frame.width() )
    CIE_OUT_OF_RANGE_CHECK( y < this->_frame.height() )
    Size index = y * this->_frame.width() + x;
    index *= this->_frame.numberOfChannels();
    return this->_frame.data()[index];
}


template <class CT>
requires (!concepts::Integer<CT>)
ValueType BadAppleScene::sampleImage( CT x, CT y ) const
{
    return this->sampleImage(
        Size( x * (this->_frame.width()-1) ),
        Size( y * (this->_frame.height()-1) )
    );
}


// -------------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------------

int main( int argc, char const* argv[] )
{
    // Parse command line arguments
    utils::CommandLineArguments args( argc, argv );
    
    // Graphics setup
    auto p_context = gl::GLFWContextSingleton::get();
    auto p_window = p_context->newWindow( 1024, 768 );
    auto p_scene = p_window->makeScene<BadAppleScene>( "BadAppleScene", args );

    auto p_camera = p_scene->getCamera();
    p_camera->setPose( {0.5, 0.5, 0.5},
                       {0.0, 0.0, -1.0},
                       {0.0, 1.0, 0.0} );
    p_camera->setClippingPlanes( 0.3, 1.1 );
    p_camera->setAspectRatio( p_window->getSize().first / p_window->getSize().second );

    auto p_controls = std::make_shared<gl::PanZoomCameraControls>();
    p_controls->bind( p_window, p_camera );
    p_controls->setMovementScale( 0.05f );
    p_controls->setZoomScale( 0.6f );

    using Clock = std::chrono::high_resolution_clock;
    auto t0 = Clock::now();

    auto checkDelay = [&t0]() -> bool
    {
        auto now = Clock::now();
        float elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - t0).count();
        if ( elapsed < 1.0/24.0 )
            return true;
        else
        {
            t0 = now;
            return false;
        }
    };

    // Collect and sort files
    auto directory = std::filesystem::path( args.arguments()[0] );
    std::set<std::string> frames;

    for ( auto& r_framePath : std::filesystem::recursive_directory_iterator(directory) )
    {
        if ( r_framePath.path().extension() == ".png" )
            frames.insert( r_framePath.path().string() );
    }

    // Main loop
    for ( const auto& r_framePath : frames )
    {
        p_scene->framePath() = r_framePath;
        p_scene->updateTree();

        while (checkDelay()) {}

        p_window->update();
    }

    return 0;
}


} // namespace cie




int main( int argc, char const* argv[] )
{
    return cie::main( argc, argv );
}