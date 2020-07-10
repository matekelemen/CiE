// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/GLContext.hpp"
#include "../../drawing/inc/Camera.hpp"
#include "../inc/DrawManager.hpp"
#include "../../callbacks/inc/CallbackGroup.hpp"

// --- STL Includes ---
#include <iostream>



namespace cie {
namespace gl {


GLContext context_global( 4,5,4,"glvisualization_testrunner_log.txt" );


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

        checkGLErrors();
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
        return true;
    }
};

/*
TEST_CASE( "GLContext" )
{

    auto loopFactory = [] (cie::gl::GLContext& context) -> cie::gl::DrawFunction
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

    REQUIRE_NOTHROW( context_global.openWindow() );
    REQUIRE_NOTHROW( context_global.makeContextCurrent() );
    REQUIRE_NOTHROW( context_global.startEventLoop( loopFactory ) );
}
*/

//TEST_CASE( "Scene" )
//{
//    Scene scene( context_global );
//    glm::vec3 test = { 5.0f, 4.0f, 3.0f };
//    scene.setCameraPose( test, test );
//}


TEST_CASE( "DrawManager" )
{
    // Create context
    REQUIRE_NOTHROW( context_global.openWindow() );
    REQUIRE_NOTHROW( context_global.makeContextCurrent() );
    
    // Create draw manager
    TestDrawManager drawManager;
    REQUIRE_NOTHROW( drawManager.makeProgram() );
    REQUIRE_NOTHROW( drawManager.initialize() );

    // Set DrawManager members
    auto camera = std::make_shared<ArcballCamera>( context_global );
    REQUIRE_NOTHROW( drawManager.camera() = camera );
    CHECK_NOTHROW( camera->setProperties( 0 ) );
    CHECK_NOTHROW( camera->setCenter( glm::vec3( 0.5f, 0.5f, 0.5f ) ) );

    // Bind callbacks
    KeyCallbackFunction keyCallback         = makeCallback( ArcballCallbacks::keyCallback,
                                                            &drawManager );
    CursorCallbackFunction cursorCallback   = makeCallback( ArcballCallbacks::cursorCallback,
                                                            &drawManager );
    MouseCallbackFunction mouseCallback     = makeCallback( ArcballCallbacks::mouseCallback,
                                                            &drawManager );

    // Start the event loop
    REQUIRE_NOTHROW(context_global.startEventLoop(  std::bind(&DrawManager::makeDrawFunction, &drawManager, std::placeholders::_1),
                                                    keyCallback,
                                                    cursorCallback,
                                                    mouseCallback     ));
}


//TEST_CASE( "Window resolution" )
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



}
}