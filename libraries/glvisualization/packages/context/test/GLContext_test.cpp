// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/GLContext.hpp"
#include "../../drawing/inc/Camera.hpp"
#include "../inc/DrawManager.hpp"
#include "../../callbacks/inc/CallbackGroup.hpp"

// --- STD Includes ---
#include <iostream>



namespace cie {
namespace gl {


GLContext context_global( 4,5,4,"glvisualization_testrunner_log.txt" );

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
    DrawManager drawManager( context_global );
    REQUIRE_NOTHROW( drawManager.makeProgram() );
    REQUIRE_NOTHROW( drawManager.initialize() );

    // Set DrawManager members
    REQUIRE_NOTHROW( drawManager.camera() = std::make_shared<InteractiveCamera>(context_global) );
    //CHECK_NOTHROW( drawManager.camera()->setProperties( 0 ) );

    // Bind callbacks
    KeyCallbackFunction keyCallback         = makeCallback( CallbackGroup::keyCallback,
                                                            &context_global, 
                                                            &drawManager );
    CursorCallbackFunction cursorCallback   = makeCallback( CallbackGroup::cursorCallback,
                                                            &context_global,
                                                            &drawManager );
    MouseCallbackFunction mouseCallback     = makeCallback( CallbackGroup::mouseCallback,
                                                            &context_global,
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