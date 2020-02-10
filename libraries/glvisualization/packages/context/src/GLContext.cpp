#include "../inc/GLContext.hpp"
#include "../inc/callbacks_error.hpp"
#include "../inc/callbacks_keys.hpp"

namespace cie {
namespace gl {



GLContext::GLContext(   uint8_t versionMajor,
                        uint8_t versionMinor,
                        uint8_t samples )   :
    _window(nullptr),
    _logger("GLContext_log.txt")
{
    // Apply settings
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMajor );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMinor );
    glfwWindowHint( GLFW_SAMPLES, samples );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    // Bind callbacks
    glfwSetErrorCallback( callback_errorPrint );

    // Check initialization
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
        _logger.error( "Failed to initialize GLFW!" );
    }
}



GLContext::~GLContext()
{
    if (_window!=nullptr)
    {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
    else 
        _logger.warn( "Attempt to destroy non-existent window while destroying the context!" );
    
    _logger.report( "Destroy context" );
    glfwTerminate();
    exit(EXIT_SUCCESS);   
}



WindowPtr GLContext::openWindow(    size_t width,
                                    size_t height,
                                    const std::string& windowName,
                                    GLFWmonitor* fullscreenMonitor,
                                    GLFWwindow* sharedWindow )
{
    if (_window != nullptr)
    {
        _logger.warn( "Open new window on an existing one, closing old window..." );
        closeWindow();
    }
    _window = glfwCreateWindow( width, 
                                height,
                                windowName.c_str(),
                                fullscreenMonitor,
                                sharedWindow)   ;
    if (!_window)
    {
        _logger.warn( "Failed to create window!" );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    else 
        _logger.report( "Open window" );

    return _window;
}



void GLContext::closeWindow()
{
    if (_window != nullptr)
    {
        glfwDestroyWindow(_window);
        _logger.report( "Close window" );
    }
    else
        _logger.warn( "Attempted to close non-existent window!" );

    _window = nullptr;
}



void GLContext::makeContextCurrent()
{
    if (_window != nullptr)
        glfwMakeContextCurrent(_window);
    else
        _logger.error( "Attempt to make non-existent window current!" );
    _logger.report( "Make context current" );
}



void GLContext::startEventLoop( )
{
    // Bind events
    if (_window != nullptr)
    {
        glfwSetKeyCallback( _window, callback_keyExit );
        _logger.report("Start event loop");
    }
    else 
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
        _logger.error( "Attempt to start event loop without an existing window!" );
    }

    // Start event loop
    while(!glfwWindowShouldClose(_window))
    {
        glfwSwapBuffers( _window );
        glfwPollEvents();
    }
}




}
}