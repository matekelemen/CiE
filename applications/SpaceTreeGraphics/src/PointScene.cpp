// --- Internal Includes ---
#include "PointScene.hpp"
#include "cmake_variables.hpp"

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- STL Includes ---
#include <vector>


namespace cie {


namespace detail {

// Shader directories
const std::filesystem::path POINT_SHADER_PATH = SOURCE_PATH / "libraries/ciegl/data/shaders/pointWithTransformation";

// Get centers of different objects

template <CubeType GeometryType>
typename csg::NodeType::point_type getCenter( const GeometryType& r_geometry )
{
    typename csg::NodeType::point_type center = r_geometry.base();

    auto tmp = r_geometry.length() / 2.0;

    for ( auto& component : center )
        component += tmp;

    return center;
}


template <BoxType GeometryType>
typename csg::NodeType::point_type getCenter( const GeometryType& r_geometry )
{
    typename csg::NodeType::point_type center = r_geometry.base();
    auto it_center = center.begin();
    auto it_lengthEnd = r_geometry.lengths().end();

    for ( auto it_length=r_geometry.lengths().begin(); it_length!=it_lengthEnd; ++it_length,++it_center )
        *it_center += (*it_length) / 2.0;

    return center;
}


} // namespace detail


PointScene::PointScene( utils::Logger& r_logger,
                        const std::string& r_name ) :
    Scene(
        r_logger,
        r_name,
        gl::makeVertexShader<gl::GLFWVertexShader>( detail::POINT_SHADER_PATH / "vertexShader.xml", detail::POINT_SHADER_PATH / "vertexShader.glsl" ),
        gl::makeGeometryShader<gl::GLFWGeometryShader>( detail::POINT_SHADER_PATH / "geometryShader.xml", detail::POINT_SHADER_PATH / "geometryShader.glsl" ),
        gl::makeFragmentShader<gl::GLFWFragmentShader>( detail::POINT_SHADER_PATH / "fragmentShader.xml", detail::POINT_SHADER_PATH / "fragmentShader.glsl" )
    ),
    _roots()
{
    auto p_camera = this->makeCamera<gl::Camera<gl::PerspectiveProjection>>();
    this->bindUniform( "transformation", p_camera->transformationMatrix() );
}


void PointScene::updatePoints()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Clear stored centers
    PointScene::vertex_attribute_container centerComponents;

    // Send lambda down the tree that collects all leaf centers
    auto getLeafCenter = [&centerComponents]( csg::NodeType* p_node ) -> bool
    {
        // Return if not a leaf node
        if ( !p_node->children().empty() )
            return true;
        else
            for ( auto& rp_child : p_node->children() )
                if ( rp_child->values().empty() )
                    return true;
        

        auto center = detail::getCenter( *p_node );
        for ( const auto& r_component : center )
            centerComponents.push_back( r_component );

        return true;
    };

    for ( auto& rp_root : this->_roots )
        rp_root->visit( getLeafCenter );

    // Write leaf centers to buffer
    this->_p_bufferManager->writeToBoundVertexBuffer( centerComponents );

    CIE_END_EXCEPTION_TRACING
}


void PointScene::update_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Get number of components to draw
    GLint64 numberOfComponents;
    glGetBufferParameteri64v( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfComponents );
    numberOfComponents /= sizeof( GLfloat );
    
    // Draw points
    glDrawArrays( GL_POINTS, 0, numberOfComponents );

    gl::checkGLErrors( "Error drawing points" );

    CIE_END_EXCEPTION_TRACING
}


PointScene& PointScene::addRoot( csg::NodePtr p_root )
{
    this->_roots.push_back( p_root );
    return *this;
}


gl::CameraPtr PointScene::getCamera()
{
    if ( this->_cameras.empty() )
        this->log( "No cameras in PointScene",
                   LOG_TYPE_ERROR );

    return *this->_cameras.begin();
}


} // namespace cie