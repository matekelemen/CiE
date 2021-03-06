/*
// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/scene/inc/Camera.hpp"
#include "ciegl/packages/drawing/inc/DrawManager.hpp"
#include "ciegl/packages/callbacks/inc/CallbackGroup.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <iostream>



namespace cie::gl {


GLFWContext context_global( 4,5,4, TEST_OUTPUT_PATH + "/ciegl_testrunner_log.txt" );


struct TestDrawManager : public DrawManager
{
    TestDrawManager() : DrawManager(context_global) {}

    void initialize() override
    {
        DrawManager::initialize();

        float cLen = 0.9f;

        std::vector<float> vertices = {
        0.0f, 0.0, 0.0f,        1.0f, 0.0f, 0.0f,
        cLen, 0.0f, 0.0f,       0.0f, 1.0f, 0.0f,
        cLen, cLen, 0.0f,       0.0f, 0.0f, 1.0f,
        0.0f, cLen, 0.0f,       1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, cLen,       1.0f, 0.0f, 1.0f,
        cLen, 0.0f, cLen,       0.0f, 1.0f, 1.0f,
        0.0f, cLen, cLen,       1.0f, 1.0f, 1.0f
        };

        std::vector<GLuint> triangles = {
            0,1,2,
            0,2,3,
            0,3,6,
            0,6,4,
            0,4,5,
            0,5,1
        };

        _buffers.writeToActiveBuffer(GL_ARRAY_BUFFER, vertices);
        _buffers.writeToActiveBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles);

        checkGLErrors( "Error while writing to buffers" );
    }

    bool draw() override
    {
        //_camera->rotate( 0.01f, glm::vec3(1.0f,-1.0f,1.0f), glm::vec3(0.0f,0.0f,0.0f) );
        DrawManager::draw();

        // Get number of elements to draw
        GLint64 numberOfElements;
        glGetBufferParameteri64v( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfElements );
        numberOfElements /= sizeof(GLuint);

        // Draw buffer
        glDrawElements( GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0 );

        return false;
    }
};


CIE_TEST_CASE( "GLFWContext" )
{

    auto loopFactory = [] (cie::gl::GLFWContext& context) -> cie::gl::DrawFunction
    {
        int* loopCounter = new int(0);

        auto function = [&context, loopCounter]() -> void
        {
            context.log( std::to_string(*loopCounter) );
            if (++(*loopCounter) >= 9 )
            {
                delete loopCounter;
                glfwSetWindowShouldClose( context.window(), 1 );
            }
        };

        return function;
    };

    CIE_TEST_REQUIRE_NOTHROW( context_global.openWindow() );
    CIE_TEST_REQUIRE_NOTHROW( context_global.makeContextCurrent() );
    CIE_TEST_REQUIRE_NOTHROW( context_global.startEventLoop( loopFactory ) );
}


//CIE_TEST_CASE( "Scene" )
//{
//    Scene scene( context_global );
//    glm::vec3 test = { 5.0f, 4.0f, 3.0f };
//    scene.setCameraPose( test, test );
//}


CIE_TEST_CASE( "DrawManager", "[ciegl]" )
{
    CIE_TEST_CASE_INIT( "DrawManager" )

    // Create context
    WindowPtr p_window;
    CIE_TEST_REQUIRE_NOTHROW( p_window = context_global.newWindow() );
    CIE_TEST_REQUIRE_NOTHROW( context_global.focusWindow( p_window ) );

    ScenePtr p_scene;
    CIE_TEST_REQUIRE_NOTHROW( p_scene = p_window->makeScene() );
    
    // Create draw manager
    TestDrawManager drawManager;
    CIE_TEST_REQUIRE_NOTHROW( drawManager.makeProgram() );
    CIE_TEST_REQUIRE_NOTHROW( drawManager.initialize() );

    // Set DrawManager members
    auto p_camera = CameraPtr(
        new ArcballCamera( context_global )
    );
    //CIE_TEST_REQUIRE_NOTHROW( drawManager.camera() = p_camera );
    CIE_TEST_REQUIRE_NOTHROW( p_scene->addCamera( p_camera ) );
    CIE_TEST_CHECK_NOTHROW( p_camera->setProperties( 0 ) );
    //CIE_TEST_CHECK_NOTHROW( p_camera->setCenter( glm::vec3( 0.5f, 0.5f, 0.5f ) ) );

    // Bind callbacks
    //KeyCallbackFunction keyCallback         = makeCallback( ArcballCallbacks::keyCallback,
    //                                                        &drawManager );
    //CursorCallbackFunction cursorCallback   = makeCallback( ArcballCallbacks::cursorCallback,
    //                                                        &drawManager );
    //MouseCallbackFunction mouseCallback     = makeCallback( ArcballCallbacks::mouseCallback,
    //                                                        &drawManager );

    // Start the event loop
    CIE_TEST_REQUIRE_NOTHROW(context_global.startEventLoop(
        std::bind(&DrawManager::makeDrawFunction, &drawManager, std::placeholders::_1)
    ));
}


//CIE_TEST_CASE( "Window resolution" )
//{
//    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
//    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 5 );
//    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
//    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
//
//    if (!glfwInit())
//        glfwTerminate();
//
//    const GLFWvidmode* mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
//
//    GLFWwindow* window = glfwCreateWindow(  mode->width, 
//                                            mode->height,
//                                            "Test Window",
//                                            glfwGetPrimaryMonitor(),
//                                            nullptr );
//
//    glfwSetFramebufferSizeCallback( window, frameBufferResizeCallback );
//
//    int width, height;
//    glfwGetWindowSize( window, &width, &height );
//    std::cout << "Requested window size:\t" << std::to_string(mode->width) << ", " << std::to_string(mode->height) << "\n";
//    std::cout << "Created window size:\t" << std::to_string(width) << ", " << std::to_string(height) << "\n";
//}



} // namespace cie::gl

*/