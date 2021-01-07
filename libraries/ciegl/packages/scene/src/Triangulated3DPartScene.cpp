// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Triangulated3DPartScene.hpp"
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <filesystem>


namespace cie::gl {


const std::filesystem::path SHADER_DIR = SOURCE_PATH / "libraries/ciegl/data/shaders/Triangulated3DPartScene";


Triangulated3DPartScene::Triangulated3DPartScene( utils::Logger& r_logger,
                                                  const std::string& r_name,
                                                  Triangulated3DPartScene::part_container&& r_parts,
                                                  CameraPtr p_camera,
                                                  ShaderPtr p_vertexShader,
                                                  ShaderPtr p_geometryShader,
                                                  ShaderPtr p_fragmentShader,
                                                  VertexBufferPtr p_vertexBuffer,
                                                  ElementBufferPtr p_elementBuffer ) :
    PartScene( r_logger,
               r_name,
               std::forward<Triangulated3DPartScene::part_container>(r_parts),
               p_camera,
               p_vertexShader,
               p_geometryShader,
               p_fragmentShader,
               p_vertexBuffer,
               p_elementBuffer )
{
}


Triangulated3DPartScene::Triangulated3DPartScene( utils::Logger& r_logger,
                                                  const std::string& r_name,
                                                  Triangulated3DPartScene::part_container&& r_parts,
                                                  CameraPtr p_camera ) :
    Triangulated3DPartScene( r_logger,
                             r_name,
                             std::forward<Triangulated3DPartScene::part_container>(r_parts),
                             p_camera,
                             gl::makeVertexShader<gl::GLFWVertexShader>( SHADER_DIR / "vertexShader.xml",
                                                                         SHADER_DIR / "vertexShader.glsl" ),
                             gl::makeGeometryShader<gl::GLFWGeometryShader>( SHADER_DIR / "geometryShader.xml",
                                                                             SHADER_DIR / "geometryShader.glsl" ),
                             gl::makeFragmentShader<gl::GLFWFragmentShader>( SHADER_DIR / "fragmentShader.xml",
                                                                             SHADER_DIR / "fragmentShader.glsl" ),
                            nullptr,
                            nullptr )
{
}


Triangulated3DPartScene::Triangulated3DPartScene( utils::Logger& r_logger,
                                                  Triangulated3DPartScene::part_container&& r_parts,
                                                  CameraPtr p_camera ) :
    Triangulated3DPartScene( r_logger,
                             "Triangulated3DPartScene",
                             std::forward<Triangulated3DPartScene::part_container>(r_parts),
                             p_camera )
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