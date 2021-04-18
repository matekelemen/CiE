// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Triangulated3DPartScene.hpp"
#include "ciegl/packages/camera/inc/Camera.hpp"
#include "ciegl/packages/camera/inc/PerspectiveProjection.hpp"
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <filesystem>


namespace cie::gl {


const std::filesystem::path SHADER_DIR = SOURCE_PATH / "libraries/ciegl/data/shaders/Triangulated3DPartScene";


Triangulated3DPartScene::Triangulated3DPartScene( const std::string& r_name,
                                                  Triangulated3DPartScene::part_container&& r_parts,
                                                  CameraPtr p_camera,
                                                  ShaderPtr p_vertexShader,
                                                  ShaderPtr p_geometryShader,
                                                  ShaderPtr p_fragmentShader,
                                                  utils::Logger& r_logger,
                                                  VertexBufferPtr p_vertexBuffer,
                                                  ElementBufferPtr p_elementBuffer ) :
    PartScene( r_name,
               std::forward<Triangulated3DPartScene::part_container>(r_parts),
               p_vertexShader,
               p_geometryShader,
               p_fragmentShader,
               r_logger,
               p_vertexBuffer,
               p_elementBuffer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Register camera or construct one if it was not provided
    if ( p_camera )
        this->addCamera( p_camera );
    else
        p_camera = this->makeCamera<Camera<PerspectiveProjection>>();

    // Bind required uniforms
    this->bindUniform( "transformation", p_camera->transformationMatrix() );
    this->bindUniform( "cameraPosition", p_camera->position() );

    CIE_END_EXCEPTION_TRACING
}


Triangulated3DPartScene::Triangulated3DPartScene( const std::string& r_name,
                                                  Triangulated3DPartScene::part_container&& r_parts,
                                                  utils::Logger& r_logger ) :
    Triangulated3DPartScene( r_name,
                             std::forward<Triangulated3DPartScene::part_container>(r_parts),
                             nullptr,
                             makeVertexShader<GLFWVertexShader>( SHADER_DIR / "vertexShader.xml",
                                                                 SHADER_DIR / "vertexShader.glsl" ),
                             makeGeometryShader<GLFWGeometryShader>( SHADER_DIR / "geometryShader.xml",
                                                                     SHADER_DIR / "geometryShader.glsl" ),
                             makeFragmentShader<GLFWFragmentShader>( SHADER_DIR / "fragmentShader.xml",
                                                                     SHADER_DIR / "fragmentShader.glsl" ),
                             r_logger,
                             nullptr,
                             nullptr )
{
}


void Triangulated3DPartScene::update_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( this->_partsRequireUpdate )
        this->updateParts();

    GLint64 numberOfIndices;
    glGetBufferParameteri64v( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfIndices );
    numberOfIndices /= sizeof( GLuint );
    glDrawElements( GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0 );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl