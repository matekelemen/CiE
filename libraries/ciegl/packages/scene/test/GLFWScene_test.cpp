// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/GLFWScene.hpp"
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/context/inc/GLFWMonitor.hpp"
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"
#include "ciegl/packages/buffer/inc/GLFWBufferManager.hpp"


namespace cie::gl {


class TestScene : public GLFWScene
{
public:
    template <class ...Args>
    TestScene( Args&&... args ) : GLFWScene( std::forward<Args>(args)... ) {}

private:
    void update_impl() override {}
};


TEST_CASE( "GLFWScene", "[scene]" )
{
    CIE_TEST_CASE_INIT( "GLFWScene" )

    // Context
    std::pair<Size,Size> glVersion { 4, 5 };
    Size MSAASamples = 0;
    auto p_context = ContextPtr(
        new GLFWContext( glVersion.first,
                         glVersion.second,
                         MSAASamples,
                         TEST_OUTPUT_PATH + "/GLFWScene_test0.txt" )
    );

    // Window
    WindowPtr p_window;
    REQUIRE_NOTHROW( p_window = p_context->newWindow() );

    // Shaders
    const std::string shaderDir   = SOURCE_PATH + "/libraries/ciegl/data/shaders";

    auto shaderPaths = [&shaderDir]( const std::string& r_shaderName )
    {
        return std::pair<std::string,std::string>(
            shaderDir + "/" + r_shaderName + ".xml",
            shaderDir + "/" + r_shaderName + ".glsl"
        );
    };

    auto p_vertexShader = makeVertexShader<GLFWVertexShader>(
        shaderPaths("defaultVertexShader").first,
        shaderPaths("defaultVertexShader").second
    );

    auto p_geometryShader = makeGeometryShader<GLFWGeometryShader>(
        shaderPaths("defaultGeometryShader").first,
        shaderPaths("defaultGeometryShader").second
    );

    auto p_fragmentShader = makeFragmentShader<GLFWFragmentShader>(
        shaderPaths("defaultFragmentShader").first,
        shaderPaths("defaultFragmentShader").second
    );

    // Buffers
    auto p_bufferManager = BufferManagerPtr(
        new GLFWBufferManager( *p_context )
    );

    VertexBufferPtr p_vertexBuffer;
    ElementBufferPtr p_elementBuffer;

    // Scene
    ScenePtr p_scene;

    {
        CIE_TEST_CASE_INIT( "create scene" )

        REQUIRE_NOTHROW( 
            p_scene = p_window->makeScene<TestScene>(
                "TestScene",
                p_vertexShader,
                p_geometryShader,
                p_fragmentShader,
                p_bufferManager )
        );

        p_vertexBuffer  = p_scene->bufferManager()->boundVertexBuffer();
        p_elementBuffer = p_scene->bufferManager()->boundElementBuffer();

        AbsVertexBuffer::data_container_type components 
        {
            0.0, 0.5, 0.0,
            0.5, -0.5, 0.0,
            -0.5, -0.5, 0.0
        };

        AbsElementBuffer::data_container_type triangles
        {
            0, 1, 2
        };

        p_bufferManager->writeToBoundVertexBuffer( components );
        p_bufferManager->writeToBoundElementBuffer( triangles );

        GLint64 numberOfElements, numberOfComponents;
        glGetBufferParameteri64v( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfElements );
        glGetBufferParameteri64v( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfComponents );
        numberOfElements /= sizeof( GLuint );
        numberOfComponents /= sizeof(GLfloat);
        std::cout << "Drawing " << numberOfComponents << " components" << std::endl;
        std::cout << "Drawing " << numberOfElements / 3.0 << " triangles" << std::endl;

        glDrawElements( GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0 );

        CHECK_NOTHROW( p_window->update() );

        std::cin.get();
    }
}


} // namespace cie::gl