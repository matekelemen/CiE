// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Incldues ---
#include "ciegl/packages/scene/inc/GenericPartScene.hpp"
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"


namespace cie::gl {


GenericPartScene::GenericPartScene( utils::Logger& r_logger,
                                    const std::string& r_name,
                                    Size drawMode,
                                    GenericPartScene::part_container&& r_parts,
                                    ShaderPtr p_vertexShader,
                                    ShaderPtr p_geometryShader,
                                    ShaderPtr p_fragmentShader,
                                    VertexBufferPtr p_vertexBuffer,
                                    ElementBufferPtr p_elementBuffer ) :
    PartScene( r_logger,
               r_name,
               std::forward<GenericPartScene::part_container>(r_parts),
               p_vertexShader,
               p_geometryShader,
               p_fragmentShader,
               p_vertexBuffer,
               p_elementBuffer ),
    _drawMode( drawMode )
{
}


GenericPartScene::GenericPartScene( utils::Logger& r_logger,
                                    const std::string& r_name,
                                    Size drawMode,
                                    const std::filesystem::path& r_shaderDirectory ) :
    GenericPartScene( r_logger,
                      r_name,
                      drawMode,
                      {},
                      makeVertexShader<GLFWVertexShader>( r_shaderDirectory / "vertexShader.xml",
                                                          r_shaderDirectory / "vertexShader.glsl" ),
                      makeGeometryShader<GLFWGeometryShader>( r_shaderDirectory / "geometryShader.xml",
                                                              r_shaderDirectory / "geometryShader.glsl" ),
                      makeFragmentShader<GLFWFragmentShader>( r_shaderDirectory / "fragmentShader.xml",
                                                              r_shaderDirectory / "fragmentShader.glsl" ),
                      nullptr,
                      nullptr )
{
}


void GenericPartScene::setDrawMode( Size drawMode )
{
    this->_drawMode = drawMode;
}


void GenericPartScene::update_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( this->_partsRequireUpdate )
        this->updateParts();

    GLint64 numberOfIndices;
    glGetBufferParameteri64v( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfIndices );
    numberOfIndices /= sizeof( GLuint );
    glDrawElements( this->_drawMode, numberOfIndices, GL_UNSIGNED_INT, 0 );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl