#include "../inc/GLContext.hpp"
#include "../inc/defaultCallback.hpp"

namespace cie {
namespace gl {



EventLoopFunction makeEmptyEventLoopFunction( GLContext& )
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
            _logger.error("Failed to initialize GLFW");
        }

        _initialized = true;
        _logger.report( "Open context" );
    }
    else
        _logger.warn( "Attempt to reinitialize the context" );
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

    terminate();
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
        terminate();
        _logger.error( "Failed to create window!" );
    }
    else
        _logger.report("Open window");
        

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
        if (!_current)
        {
            glfwMakeContextCurrent(_window);
            _logger.report( "Make context current" );
            _current = true;
        }
        else
            _logger.warn( "Attempt to make an existing current context current" );

        if ((!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
        {
            terminate();
            _logger.error("Failed to initialize GLAD");
        }
        else
            _logger.report("Initialize GLAD");

        glDebugMessageCallback( messageCallback, &_logger );
    }
    else
        _logger.error( "Attempt to make non-existent window current!" );
}



void GLContext::startEventLoop( EventLoopFunctionFactory eventLoopGenerator,
                                KeyCallbackFunction keyCallback )
{
    // Bind loop
    _eventLoop = eventLoopGenerator( *this );

    // Bind events
    if (_window != nullptr)
    {
        glfwSetKeyCallback( _window, keyCallback );
        _logger.report("Start event loop");
    }
    else 
    {
        terminate();
        _logger.error( "Attempt to start event loop without an existing window!" );
    }

    // Start event loop
    while(!glfwWindowShouldClose(_window))
    {
        _eventLoop();
        glfwPollEvents();
        glfwSwapBuffers( _window );
    }

    _logger.report( "End event loop" );
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

    glfwTerminate();
    _logger.report( "Terminate context" );
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