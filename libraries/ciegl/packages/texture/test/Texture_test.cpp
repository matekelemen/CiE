// --- External Includes ---
#include "glm/glm.hpp"

// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/texture/inc/Texture.hpp"
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/utility/inc/paths.hpp"
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"

// --- STL Includes ---
#include <chrono>
#include <thread>


namespace cie::gl {


class TextureTestScene : public Scene
{
public:
    template <class ...Args>
    TextureTestScene( Args&&... args ) : Scene( std::forward<Args>(args)... ) {}

private:
    void update_impl() override
    {
        GLint64 numberOfIndices;
        glGetBufferParameteri64v( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfIndices );
        numberOfIndices /= sizeof( GLuint );
        glDrawElements( GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0 );
    }
};



CIE_TEST_CASE( "Texture", "[texture]" )
{
    CIE_TEST_CASE_INIT( "Texture" )

    auto p_context = gl::GLFWContextSingleton::get();
    auto p_window  = p_context->newWindow();

    auto shaderPath = GL_SHADER_SOURCE_PATH / "defaultWithTexture";

    auto p_vertexShader = makeVertexShader<GLFWVertexShader>(
        shaderPath / "vertexShader.xml",
        shaderPath / "vertexShader.glsl"
    );

    auto p_geometryShader = makeGeometryShader<GLFWGeometryShader>(
        shaderPath / "geometryShader.xml",
        shaderPath / "geometryShader.glsl"
    );

    auto p_fragmentShader = makeFragmentShader<GLFWFragmentShader>(
        shaderPath / "fragmentShader.xml",
        shaderPath / "fragmentShader.glsl"
    );

    auto p_scene = p_window->makeScene<TextureTestScene>(
        "TextureTestScene",
        p_vertexShader,
        p_geometryShader,
        p_fragmentShader
    );

    glm::mat4 transformationMatrix( 1.0f );
    Image image( GL_TEXTURE_SOURCE_PATH / "cage.png", true );

    p_scene->bindUniform( "transformation", transformationMatrix );
    p_scene->loadTexture( "image", image );

    const VertexBuffer::data_type tmp = 0.5;
    VertexBuffer::data_container_type components {
        -tmp, -tmp, -tmp, 0.0, 0.0,
        tmp, -tmp, -tmp, 1.0, 0.0,
        -tmp, tmp, -tmp, 0.0, 1.0,
        tmp, tmp, -tmp, 1.0, 1.0
    };

    ElementBuffer::data_container_type triangles {
        0, 1, 3,
        0, 3, 2
    };

    p_scene->bufferManager()->writeToBoundVertexBuffer( components );
    p_scene->bufferManager()->writeToBoundElementBuffer( triangles );

    p_window->update();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}


} // namespace cie::gl