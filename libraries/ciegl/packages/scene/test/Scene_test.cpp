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
    TestScene( Args&&... args ) : Scene( std::forward<Args>(args)... ) 
    {
        glEnable(GL_DEPTH_TEST);
    }

private:
    void update_impl() override
    {
        GLint64 numberOfIndices;
        glGetBufferParameteri64v( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfIndices );
        numberOfIndices /= sizeof( GLuint );
        glDrawElements( GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0 );
    }
};


TEST_CASE( "Scene", "[scene]" )
{
    CIE_TEST_CASE_INIT( "Scene" )

    // Context
    std::pair<Size,Size> glVersion { 4, 5 };
    Size MSAASamples = 2;
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
        shaderPaths("default/vertexShader").first,
        shaderPaths("default/vertexShader").second
    );

    auto p_geometryShader = makeGeometryShader<GLFWGeometryShader>(
        shaderPaths("default/geometryShader").first,
        shaderPaths("default/geometryShader").second
    );

    auto p_fragmentShader = makeFragmentShader<GLFWFragmentShader>(
        shaderPaths("default/fragmentShader").first,
        shaderPaths("default/fragmentShader").second
    );

    // Scene
    ScenePtr p_scene;
    BufferManagerPtr p_bufferManager;

    {
        CIE_TEST_CASE_INIT( "create scene" )
        auto localBlock = p_context->newBlock( "basic scene" );

        REQUIRE_NOTHROW( 
            p_scene = p_window->makeScene<TestScene>(
                "TestScene",
                p_vertexShader,
                p_geometryShader,
                p_fragmentShader )
        );

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

        REQUIRE_NOTHROW( p_bufferManager = p_scene->bufferManager() );
        p_bufferManager->writeToBoundVertexBuffer( components );
        p_bufferManager->writeToBoundElementBuffer( triangles );

        CHECK_NOTHROW( p_window->update() );

        CHECK_NOTHROW( p_window->removeScene(p_scene) );
    }

    {
        CIE_TEST_CASE_INIT( "multiple scenes" )
        auto localBlock = p_context->newBlock( "two scenes" );

        ScenePtr p_secondScene;

        CHECK_NOTHROW(
            p_scene = p_window->makeScene<TestScene>(
                "TestScene0",
                p_vertexShader,
                p_geometryShader,
                p_fragmentShader
            )
        );

        CHECK_NOTHROW(
            p_secondScene = p_window->makeScene<TestScene>(
                "TestScene1",
                p_vertexShader,
                p_geometryShader,
                p_fragmentShader
            )
        );

        AbsVertexBuffer::data_container_type components0 
        {
            0.0, 0.5, 0.0,
            0.5, -0.5, 0.0,
            0.0, -0.5, 0.0
        };

        AbsElementBuffer::data_container_type triangles0
        {
            0, 1, 2
        };

        AbsVertexBuffer::data_container_type components1
        {
            0.0, 0.5, 0.0,
            0.5, 0.5, 0.0,
            0.5, -0.5, 0.0
        };

        AbsElementBuffer::data_container_type triangles1
        {
            0, 1, 2
        };

        p_scene->bufferManager()->writeToBoundVertexBuffer( components0 );
        p_scene->bufferManager()->writeToBoundElementBuffer( triangles0 );
        
        p_secondScene->bufferManager()->writeToBoundVertexBuffer( components1 );
        p_secondScene->bufferManager()->writeToBoundElementBuffer( triangles1 );

        CHECK_NOTHROW( p_window->update() );
        std::this_thread::sleep_for( std::chrono::milliseconds(500) );

        CHECK_NOTHROW( p_window->removeScene( p_scene ) );
        CHECK_NOTHROW( p_window->removeScene( p_secondScene ) );
    }



    // Scene with uniforms
    p_vertexShader = makeVertexShader<GLFWVertexShader>(
        shaderPaths("trianglesWithNormals/vertexShader").first,
        shaderPaths("trianglesWithNormals/vertexShader").second
    );

    p_geometryShader = makeGeometryShader<GLFWGeometryShader>(
        shaderPaths("trianglesWithNormals/geometryShader").first,
        shaderPaths("trianglesWithNormals/geometryShader").second
    );

    p_fragmentShader = makeFragmentShader<GLFWFragmentShader>(
        shaderPaths("trianglesWithNormals/fragmentShader").first,
        shaderPaths("trianglesWithNormals/fragmentShader").second
    );

    auto p_wireframeVertexShader = makeVertexShader<GLFWVertexShader>(
        shaderPaths("wireframeWithNormal/vertexShader").first,
        shaderPaths("wireframeWithNormal/vertexShader").second
    );

    auto p_wireframeGeometryShader = makeGeometryShader<GLFWGeometryShader>(
        shaderPaths("wireframeWithNormal/geometryShader").first,
        shaderPaths("wireframeWithNormal/geometryShader").second
    );

    auto p_wireframeFragmentShader = makeFragmentShader<GLFWFragmentShader>(
        shaderPaths("wireframeWithNormal/fragmentShader").first,
        shaderPaths("wireframeWithNormal/fragmentShader").second
    );


    {
        CIE_TEST_CASE_INIT( "scene with uniforms" )
        auto localBlock = p_context->newBlock( "scene with uniforms" );

        // Primary scene for triangles
        REQUIRE_NOTHROW( 
            p_scene = p_window->makeScene<TestScene>(
                "Scene_triangles",
                p_vertexShader,
                p_geometryShader,
                p_fragmentShader )
        );

        // Secondary scene for the wireframe
        ScenePtr p_secondScene;
        REQUIRE_NOTHROW(
            p_secondScene = p_window->makeScene<TestScene>(
                "Scene_wireframe"
                ,p_wireframeVertexShader
                ,p_wireframeGeometryShader
                ,p_wireframeFragmentShader
                ,p_scene->bufferManager()->boundVertexBuffer()
                ,p_scene->bufferManager()->boundElementBuffer()
                )
        );

        CHECK( p_scene->bufferManager()->boundVertexBuffer() == p_secondScene->bufferManager()->boundVertexBuffer() );
        CHECK( p_scene->bufferManager()->boundElementBuffer() == p_secondScene->bufferManager()->boundElementBuffer() );

        const float a = 0.5;

        typename AbsVertexBuffer::data_container_type vertexData
        {
            // position ----------- normal
            a, 0.0, 0.0,    1.0, 0.0, 0.0,  // 1 
            a, a, 0.0,      1.0, 0.0, 0.0,  // 3
            a, a, a,        1.0, 0.0, 0.0,  // 7
            a, a, a,        1.0, 0.0, 0.0,  // 7
            a, 0.0, a,      1.0, 0.0, 0.0,  // 5
            a, 0.0, 0.0,    1.0, 0.0, 0.0,  // 1

            0.0, 0.0, 0.0,  -1.0, 0.0, 0.0, // 0
            0.0, 0.0, a,    -1.0, 0.0, 0.0, // 4
            0.0, a, a,      -1.0, 0.0, 0.0, // 6
            0.0, a, a,      -1.0, 0.0, 0.0, // 6
            0.0, a, 0.0,    -1.0, 0.0, 0.0, // 2
            0.0, 0.0, 0.0,  -1.0, 0.0, 0.0, // 0

            0.0, a, 0.0,    0.0, 1.0, 0.0,  // 2
            0.0, a, a,      0.0, 1.0, 0.0,  // 6
            a, a, a,        0.0, 1.0, 0.0,  // 7
            a, a, a,        0.0, 1.0, 0.0,  // 7
            a, a, 0.0,      0.0, 1.0, 0.0,  // 3
            0.0, a, 0.0,    0.0, 1.0, 0.0,  // 2

            0.0, 0.0, 0.0,  0.0, -1.0, 0.0, // 0
            a, 0.0, 0.0,    0.0, -1.0, 0.0, // 1
            a, 0.0, a,      0.0, -1.0, 0.0, // 5
            a, 0.0, a,      0.0, -1.0, 0.0, // 5
            0.0, 0.0, a,    0.0, -1.0, 0.0, // 4
            0.0, 0.0, 0.0,  0.0, -1.0, 0.0, // 0

            0.0, 0.0, a,    0.0, 0.0, 1.0,  // 4
            a, 0.0, a,      0.0, 0.0, 1.0,  // 5
            a, a, a,        0.0, 0.0, 1.0,  // 7
            a, a, a,        0.0, 0.0, 1.0,  // 7
            0.0, a, a,      0.0, 0.0, 1.0,  // 6
            0.0, 0.0, a,    0.0, 0.0, 1.0,  // 4

            0.0, 0.0, 0.0,  0.0, 0.0, -1.0, // 0
            0.0, a, 0.0,    0.0, 0.0, -1.0, // 2
            a, a, 0.0,      0.0, 0.0, -1.0, // 3
            a, a, 0.0,      0.0, 0.0, -1.0, // 3
            a, 0.0, 0.0,    0.0, 0.0, -1.0, // 1
            0.0, 0.0, 0.0,  0.0, 0.0, -1.0  // 0
        };

        // Cube triangles
        AbsElementBuffer::data_container_type triangles
        {
            0, 1, 2,        3, 4, 5,
            6, 7, 8,        9, 10, 11,
            12, 13, 14,     15, 16, 17,
            18, 19, 20,     21, 22, 23,
            24, 25, 26,     27, 28, 29,
            30, 31, 32,     33, 34, 35
        };

        REQUIRE_NOTHROW( p_bufferManager = p_scene->bufferManager() );
        p_bufferManager->writeToBoundVertexBuffer( vertexData );
        p_bufferManager->writeToBoundElementBuffer( triangles );

        using CameraType = Camera<PerspectiveProjection>;
        auto p_camera = p_scene->makeCamera<CameraType>();
        p_secondScene->addCamera( p_camera );

        p_camera->setAspectRatio( p_window->getSize().first / double(p_window->getSize().second) );
        p_camera->setPosition( {a/2.0, a/2.0, 2.0*a + p_camera->clippingPlanes().first} );
        p_camera->setFieldOfView( 60.0 * M_PI / 180.0 );

        CameraType::vector_type center { a/2.0, a/2.0, a/2.0 };

        REQUIRE_NOTHROW( p_scene->bindUniform( "transformation", p_camera->transformationMatrix() ) );
        REQUIRE_NOTHROW( p_scene->bindUniform( "cameraPosition", p_camera->position() ) );

        GLfloat wireframeScale = 1e-4;
        REQUIRE_NOTHROW( p_secondScene->bindUniform( "transformation", p_camera->transformationMatrix() ) );
        REQUIRE_NOTHROW( p_secondScene->bindUniform( "wireframeScale", wireframeScale ) );

        const int steps = 360;
        std::chrono::microseconds delay( 100 );

        // Translate
        for ( int i=0; i<steps; ++i )
        {
            p_camera->translate( { i<steps/2 ? 0.01 : -0.01 ,0.0,0.0} );
            CHECK_NOTHROW( p_window->update() );
            std::this_thread::sleep_for( delay );
        }

        // Rotate
        for ( int i=0; i<steps; ++i )
        {
            p_camera->rotate( M_PI / 180.0,
                              { 0.0, 0.0, 1.0 },
                              center );
            CHECK_NOTHROW( p_window->update() );
            std::this_thread::sleep_for( delay );
        }

        // Rotate
        for ( int i=0; i<steps; ++i )
        {
            p_camera->rotate( (i<steps/2 ? 1.0 : -1.0) * M_PI / 180.0,
                              { 1.0, 0.0, 0.0 },
                              center );
            CHECK_NOTHROW( p_window->update() );
            std::this_thread::sleep_for( delay );
        }

        // Zoom
        for ( int i=0; i<steps; ++i )
        {
            p_camera->zoom( i<steps/2 ? 1.01 : 0.99 );
            CHECK_NOTHROW( p_window->update() );
            std::this_thread::sleep_for( delay );
        }

        CHECK_NOTHROW( p_window->removeScene(p_scene) );
        CHECK_NOTHROW( p_window->removeScene(p_secondScene) );
    }
}


} // namespace cie::gl