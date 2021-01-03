// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/plot/inc/Plot2.hpp"
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"
#include "ciegl/packages/utility/inc/GLError.hpp"
#include "ciegl/packages/camera/inc/camera_utilities.hpp"


namespace cie::gl {


/* --- AbsPlot2 --- */

AbsPlot2::AbsPlot2( WindowPtr p_window ) :
    AbsPlot( p_window ),
    _vertices(),
    _p_camera( nullptr ),
    _p_scene( nullptr ),
    _p_controls( nullptr )
{
    this->initializeScene();
}


AbsPlot2::AbsPlot2() :
    AbsPlot(),
    _vertices(),
    _p_camera( nullptr ),
    _p_scene( nullptr ),
    _p_controls( nullptr )
{
    this->initializeScene();
}


void AbsPlot2::fit()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Get data domain
    double xMin = std::numeric_limits<double>().max();
    double xMax = std::numeric_limits<double>().min();
    double yMin = std::numeric_limits<double>().max();
    double yMax = std::numeric_limits<double>().min();

    for ( const auto& rp_vertex : this->_vertices )
    {
        if ( rp_vertex->x() < xMin )
            xMin = rp_vertex->x();
        if ( xMax < rp_vertex->x() )
            xMax = rp_vertex->x();

        if ( rp_vertex->y() < yMin )
            yMin = rp_vertex->y();
        if ( yMax < rp_vertex->y() )
            yMax = rp_vertex->y();
    }

    // Scale data
    auto makeNormalizeFunctor = []( double minValue, double maxValue )
    {
        CIE_CHECK( minValue < maxValue, "Invalid domain" )
        double offset = minValue;
        double scale  = maxValue - minValue;

        return [offset,scale]( AbsPlot2::vertex_type::value_type& r_component ) -> void
        {
            r_component -= offset;
            r_component /= scale;
        };
    };

    auto normalizeX = makeNormalizeFunctor( xMin, xMax );
    auto normalizeY = makeNormalizeFunctor( yMin, yMax );

    for ( auto& rp_vertex : this->_vertices )
    {
        normalizeX( rp_vertex->x() );
        normalizeY( rp_vertex->y() );
    }

    this->_p_scene->setUpdateFlag();

    // Adjust camera
    using BoxType = csg::Box<3>;
    BoxType box( BoxType::point_type { 0.0, 0.0, -0.5 },
                 BoxType::point_type { 1.0, 1.0, 1.0 } );

    this->_p_camera->setPosition( { 0.5, 0.5, 2.0 } );
    this->_p_camera->setClippingPlanes( 0.1, 4.0 );

    fitViewToBox<OrthographicProjection>( *this->_p_camera, box );

    // Adjust controls
    this->_p_controls->setMovementScale( 0.1 );

    CIE_END_EXCEPTION_TRACING
}


void AbsPlot2::initializeScene()
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_scene = std::make_shared<AbsPlot2::Plot2Scene>( *this->_p_context,
                                                        "Plot2Scene",
                                                        this->_p_attributes );

    this->_p_scene = p_scene;
    this->_p_window->addScene( p_scene );
    
    this->_p_camera = p_scene->getCamera();
    this->_p_camera->setPose( { 0.0, 0.0, 1.0 },
                              { 0.0, 0.0, -1.0 },
                              { 0.0, 1.0, 0.0 } );
    this->_p_camera->setClippingPlanes( 0.5, 1.5 );

    this->_p_controls = AbsPlot2::controls_ptr(
        new AbsPlot2::controls_type(true)
    );
    this->_p_controls->bind( this->_p_window, this->_p_camera );

    CIE_END_EXCEPTION_TRACING
}


/* --- AbsPlot2::Plot2Scene --- */

const std::filesystem::path CURRENT_SHADER_DIR = SOURCE_PATH / "libraries/ciegl/data/shaders/plot2";


AbsPlot2::Plot2Scene::Plot2Scene( utils::Logger& r_logger,
                               const std::string& r_name,
                               AttributeContainerPtr p_attributes ) :
    Scene(
        r_logger,
        r_name,
        makeVertexShader<GLFWVertexShader>( CURRENT_SHADER_DIR / "vertexShader.xml",
                                            CURRENT_SHADER_DIR / "vertexShader.glsl" ),
        nullptr,
        makeFragmentShader<GLFWFragmentShader>( CURRENT_SHADER_DIR / "fragmentShader.xml",
                                                CURRENT_SHADER_DIR / "fragmentShader.glsl" )
    ),
    _p_attributes( p_attributes ),
    _updateFlag( true )
{
    CIE_BEGIN_EXCEPTION_TRACING
    
    auto p_camera = this->makeCamera<AbsPlot2::camera_type>();
    this->bindUniform( "transformation", p_camera->transformationMatrix() );

    CIE_END_EXCEPTION_TRACING
}


CameraPtr AbsPlot2::Plot2Scene::getCamera()
{
    return *this->_cameras.begin();
}


void AbsPlot2::Plot2Scene::setUpdateFlag()
{
    this->_updateFlag = true;
}


void AbsPlot2::Plot2Scene::update_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Write attributes into the buffer if requested
    if ( this->_updateFlag )
    {
        this->_p_bufferManager->writeToBoundVertexBuffer( *this->_p_attributes );
        this->_updateFlag = false;
    }

    checkGLErrors( *this, "Error writing to buffer" );

    GLint64 numberOfPoints;
    glGetBufferParameteri64v( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfPoints );
    numberOfPoints /= sizeof( GLfloat );
    numberOfPoints /= 2;

    glDrawArrays( GL_LINE_STRIP, 0, numberOfPoints );

    CIE_END_EXCEPTION_TRACING
}




} // namespace cie::gl