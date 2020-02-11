#include "../inc/GLContext.hpp"
#include "../inc/callbacks_error.hpp"

namespace cie {
namespace gl {



EventLoopFunction makeEmptyEventLoopFunction( GLContext& )
{
    auto function = [&]() -> void {  };
    return function;
}




GLContext::GLContext(   uint8_t versionMajor,
                        uint8_t versionMinor,
                        uint8_t samples,
                        EventLoopFunctionFactory loopFactory,
                        const std::string& logFileName  )   :
    _window(nullptr),
    _logger(logFileName)
{
    // Apply settings
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMajor );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMinor );
    glfwWindowHint( GLFW_SAMPLES, samples );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    // Bind callbacks and event loops
    glfwSetErrorCallback( callback_errorPrint );
    _eventLoop = loopFactory( *this );

    // Check initialization
    if (!glfwInit())
    {
        //terminate(EXIT_FAILURE);
        _logger.error( "Failed to initialize GLFW!" );
    }

    _logger.report( "Open context" );
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
    
    glfwTerminate();
    _logger.report( "Destroy context" );
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
    {
        glfwMakeContextCurrent(_window);
        _logger.report( "Make context current" );
    }
    else
        _logger.error( "Attempt to make non-existent window current!" );
}



void GLContext::startEventLoop( KeyCallbackFunction keyCallback )
{
    // Bind events
    if (_window != nullptr)
    {
        glfwSetKeyCallback( _window, keyCallback );
        _logger.report("Start event loop");
    }
    else 
    {
        glfwTerminate();
        _logger.error( "Attempt to start event loop without an existing window!" );
    }

    // Start event loop
    while(!glfwWindowShouldClose(_window))
    {
        _eventLoop();
        glfwSwapBuffers( _window );
        glfwPollEvents();
    }

    _logger.report( "End event loop" );
}



GLLogger& GLContext::logger()
{
    _logger.report( "External log report:" );
    return _logger;
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