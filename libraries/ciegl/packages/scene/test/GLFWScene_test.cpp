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

        CHECK_NOTHROW( p_window->update() );

        CHECK_NOTHROW( p_window->removeScene(p_scene) );
    }

//    {
//        CIE_TEST_CASE_INIT( "multiple scenes" )
//        // TODO
//        ScenePtr p_secondScene;
//
//        CHECK_NOTHROW(
//            p_scene = p_window->makeScene<TestScene>(
//                "TestScene0",
//                p_vertexShader,
//                p_geometryShader,
//                p_fragmentShader,
//                p_bufferManager
//            )
//        );
//
//        AbsVertexBuffer::data_container_type components0 
//        {
//            0.0, 0.5, 0.0,
//            0.5, -0.5, 0.0,
//            -0.5, -0.5, 0.0
//        };
//
//        AbsElementBuffer::data_container_type triangles0
//        {
//            0, 1, 2
//        };
//
//        CHECK_NOTHROW(
//            p_secondScene = p_window->makeScene<TestScene>(
//                "TestScene1",
//                p_vertexShader,
//                p_geometryShader,
//                p_fragmentShader,
//                p_bufferManager
//            )
//        );
//    }
}


} // namespace cie::gl