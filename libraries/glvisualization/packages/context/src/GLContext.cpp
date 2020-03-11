#include "../inc/GLContext.hpp"
#include "../inc/defaultCallback.hpp"

#include <string>
#include <iostream>

namespace cie {
namespace gl {



DrawFunction makeEmptyDrawFunction( GLContext& )
{
    auto function = [&]() -> void {  };
    return function;
}



bool GLContext::_initialized    = false;
bool GLContext::_active         = false;
bool GLContext::_current        = false;




GLContext::GLContext(   uint8_t versionMajor,
                        uint8_t versionMinor,
                        uint8_t samples,
                        const std::string& logFileName  )   :
    _window(nullptr),
    _logger(logFileName)
{
    if (!_initialized)
    {
        // Apply settings
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMajor );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMinor );
        glfwWindowHint( GLFW_SAMPLES, samples );
        //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );

        // Bind callbacks and event loops
        glfwSetErrorCallback( callback_errorPrint );

        if (!glfwInit())
        {
            terminate();
            log("Failed to initialize GLFW", CONTEXT_LOG_TYPE_ERROR );
        }

        _initialized = true;
        log( "Open context" );
    }
    else
        log( "Attempt to reinitialize the context", CONTEXT_LOG_TYPE_WARNING );
}



GLContext::~GLContext()
{
    terminate();
    log( "Destroy context" );
}



WindowPtr GLContext::openWindow(    size_t width,
                                    size_t height,
                                    const std::string& windowName,
                                    GLFWmonitor* fullscreenMonitor,
                                    GLFWwindow* sharedWindow )
{
    if (_window != nullptr)
    {
        log( "Open new window on an existing one, closing old window...", CONTEXT_LOG_TYPE_WARNING );
        closeWindow();
    }
    _window = glfwCreateWindow( width, 
                                height,
                                windowName.c_str(),
                                fullscreenMonitor,
                                sharedWindow)   ;
    if (!_window)
    {
        terminate();
        log( "Failed to create window!", CONTEXT_LOG_TYPE_ERROR );
    }
    else
        log("Open window");

    // Set basic callbacks
    glfwSetFramebufferSizeCallback( _window, frameBufferResizeCallback );
        
    // Check window dimensions
    int checkWidth, checkHeight;
    glfwGetFramebufferSize( _window, &checkWidth, &checkHeight );
    if ( (size_t)checkWidth!=width || (size_t)checkHeight!=height )
        log( "Created window is not of the requested size!", CONTEXT_LOG_TYPE_WARNING );

    return _window;
}



void GLContext::closeWindow()
{
    _current = false;

    if (_window != nullptr)
    {
        glfwSetWindowShouldClose( _window, 1 );
        log( "Close window" );
    }
    else
        log( "Attempt to close non-existent window!", CONTEXT_LOG_TYPE_WARNING );

    _window = nullptr;
}



void GLContext::makeContextCurrent()
{
    if (_window != nullptr)
    {
        if (!_current)
        {
            glfwMakeContextCurrent(_window);
            log( "Make context current" );
            _current = true;
        }
        else
            log( "Attempt to make an existing current context current", CONTEXT_LOG_TYPE_WARNING );

        if ((!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
        {
            terminate();
            log( "Failed to initialize GLAD", CONTEXT_LOG_TYPE_ERROR );
        }
        else
            log("Initialize GLAD");

        glDebugMessageCallback( messageCallback, &_logger );
    }
    else
        log( "Attempt to make non-existent window current!", CONTEXT_LOG_TYPE_ERROR );
}



void GLContext::startEventLoop( DrawFunctionFactory eventLoopGenerator,
                                KeyCallbackFunction keyCallback )
{
    // Bind loop
    _drawFunction = eventLoopGenerator( *this );

    // Bind events
    if (_window != nullptr)
    {
        glfwSetKeyCallback( _window, keyCallback );
        log("Start event loop");
    }
    else 
    {
        terminate();
        log(    "Attempt to start event loop without an existing window!", 
                CONTEXT_LOG_TYPE_ERROR );
    }

    // Start event loop
    while(!glfwWindowShouldClose(_window))
    {
        _drawFunction();

        // Check errors
        GLuint error = glGetError();
        if (error!=0)
            log( "Error drawing! Error code: " + std::to_string(error), CONTEXT_LOG_TYPE_ERROR );

        glfwPollEvents();

        // Check errors
        error = glGetError();
        if (error!=0)
            log( "Error polling events! Error code: " + std::to_string(error), CONTEXT_LOG_TYPE_ERROR );
        
        glfwSwapBuffers( _window );

        // Check errors
        error = glGetError();
        if (error!=0)
            log( "Error swapping buffers! Error code: " + std::to_string(error), CONTEXT_LOG_TYPE_ERROR );
    }

    log( "End event loop" );
    closeWindow();
}



void GLContext::log(    const std::string& message,
                        GLuint messageType )
{
    switch( messageType )
    {
        case CONTEXT_LOG_TYPE_REPORT:
            _logger.report( message );
            break;
        
        case CONTEXT_LOG_TYPE_WARNING:
            _logger.warn( message );
            break;
        
        case CONTEXT_LOG_TYPE_ERROR:
            _logger.error( message );
    }
}


void GLContext::terminate()
{
    closeWindow();
    glfwTerminate();
    log( "Terminate context" );
}



WindowPtr GLContext::window()
{
    return _window;
}



const WindowPtr GLContext::window() const
{
    return _window;
}




}
}