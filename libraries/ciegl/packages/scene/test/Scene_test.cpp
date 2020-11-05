// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Scene.hpp"
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/context/inc/GLFWMonitor.hpp"
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"
#include "ciegl/packages/buffer/inc/GLFWBufferManager.hpp"
#include "ciegl/packages/camera/inc/Camera.hpp"
#include "ciegl/packages/camera/inc/PerspectiveProjection.hpp"
#include "ciegl/packages/camera/inc/OrthographicProjection.hpp"

// --- STL Includes ---
#include <chrono>
#include <thread>


namespace cie::gl {


class TestScene : public Scene
{
public:
    template <class ...Args>
    TestScene( Args&&... args ) : Scene( std::forward<Args>(args)... ) {}
};


TEST_CASE( "Scene", "[scene]" )
{
    CIE_TEST_CASE_INIT( "Scene" )

    // Context
    std::pair<Size,Size> glVersion { 4, 5 };
    Size MSAASamples = 0;
    auto p_context = ContextPtr(
        new GLFWContext( glVersion.first,
                         glVersion.second,
                         MSAASamples,
                         TEST_OUTPUT_PATH + "/GLFWScene_test.txt" )
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
        auto localBlock = p_context->newBlock( "basic scene" );

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



    // Scene with uniforms
    p_vertexShader = makeVertexShader<GLFWVertexShader>(
        shaderPaths("pointVertexShader").first,
        shaderPaths("pointVertexShader").second
    );

    p_geometryShader = makeGeometryShader<GLFWGeometryShader>(
        shaderPaths("defaultGeometryShader").first,
        shaderPaths("defaultGeometryShader").second
    );

    p_fragmentShader = makeFragmentShader<GLFWFragmentShader>(
        shaderPaths("pointFragmentShader").first,
        shaderPaths("pointFragmentShader").second
    );

    // Buffers
    p_bufferManager = BufferManagerPtr(
        new GLFWBufferManager( *p_context )
    );

    {
        CIE_TEST_CASE_INIT( "scene with uniforms" )
        auto localBlock = p_context->newBlock( "scene with uniforms" );

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
            0.0, -0.5, 0.0,
            0.5, -0.5, 0.0
        };

        AbsElementBuffer::data_container_type triangles
        {
            0, 1, 2
        };

        p_bufferManager->writeToBoundVertexBuffer( components );
        p_bufferManager->writeToBoundElementBuffer( triangles );

        auto p_camera = p_scene->makeCamera<Camera<PerspectiveProjection>>();
        p_camera->setAspectRatio( p_window->getSize().first / double(p_window->getSize().second) );
        p_camera->setPosition( {0.0, 0.0, 1.5} );
        p_camera->setFieldOfView( 10.0 );

        CHECK_NOTHROW( p_scene->bindUniform( "transformation", p_camera->transformationMatrix() ) );

        for ( int i=0; i<360; ++i )
        {
            //p_camera->translate( {0.1,0.0,0.0} );
            p_camera->rotate( M_PI / 180.0,
                              { 0.0, 0.0, 1.0 } );
            CHECK_NOTHROW( p_window->update() );
            std::this_thread::sleep_for( std::chrono::milliseconds(10) );
        }

        CHECK_NOTHROW( p_window->removeScene(p_scene) );
    }
}


} // namespace cie::gl