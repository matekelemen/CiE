// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLContext.hpp"
#include "ciegl/packages/context/inc/defaultCallback.hpp"

// --- STL Includes ---
#include <string>
#include <iostream>

namespace cie::gl {



DrawFunction makeEmptyDrawFunction( GLContext& )
{
    auto function = [&]() -> bool { return true; };
    return function;
}



bool GLContext::_initialized    = false;
bool GLContext::_active         = false;
bool GLContext::_current        = false;




GLContext::GLContext(   uint8_t versionMajor,
                        uint8_t versionMinor,
                        uint8_t samples,
                        const std::string& logFileName  )   :
    utils::Logger( logFileName ),
    utils::AbsSubject(  ),
    _window(nullptr)
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
            error( "Failed to initialize GLFW" );
        }

        _initialized = true;
        log( "Open context" );
    }
    else
        warn( "Attempt to reinitialize the context" );
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
        warn( "Open new window on an existing one, closing old window..." );
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
        error( "Failed to create window!" );
    }
    else
        log("Open window");

    // Set basic callbacks
    glfwSetFramebufferSizeCallback( _window, frameBufferResizeCallback );
        
    // Check window dimensions
    int checkWidth, checkHeight;
    glfwGetFramebufferSize( _window, &checkWidth, &checkHeight );
    if ( (size_t)checkWidth!=width || (size_t)checkHeight!=height )
        warn( "Created window is not of the requested size!" );

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
            warn( "Attempt to make an existing current context current" );

        if ((!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
        {
            terminate();
            error( "Failed to initialize GLAD" );
        }
        else
            log("Initialize GLAD");

        glDebugMessageCallback( messageCallback, this );
    }
    else
        error( "Attempt to make non-existent window current!" );
}



void GLContext::startEventLoop( DrawFunctionFactory eventLoopGenerator,
                                KeyCallbackFunction keyCallback,
                                CursorCallbackFunction cursorCallback,
                                MouseCallbackFunction mouseCallback )
{
    // Bind loop
    _drawFunction = eventLoopGenerator( *this );

    // Bind events
    if (_window != nullptr)
    {
        glfwSetKeyCallback( _window, keyCallback );
        glfwSetCursorPosCallback( _window, cursorCallback );
        glfwSetMouseButtonCallback( _window, mouseCallback );
        separate();
        log("Start event loop");
    }
    else 
    {
        terminate();
        error( "Attempt to start event loop without an existing window!" );
    }

    // Start event loop
    while(!glfwWindowShouldClose(_window))
    {
        if ( !_drawFunction() )
            glfwSetWindowShouldClose(_window, GLFW_TRUE);

        // Check errors
        GLuint err = glGetError();
        if (err!=0)
            error( "Error drawing! Error code: " + std::to_string(err) );

        glfwPollEvents();

        // Check errors
        err = glGetError();
        if (err!=0)
            error( "Error polling events! Error code: " + std::to_string(err) );
        
        glfwSwapBuffers( _window );

        // Check errors
        err = glGetError();
        if (err!=0)
            error( "Error swapping buffers! Error code: " + std::to_string(err) );
    }

    log( "End event loop" );
    separate();
    closeWindow();
}


void GLContext::terminate()
{
    log( "Terminate context" );
    closeWindow();
    glfwTerminate();
}



WindowPtr GLContext::window()
{
    return _window;
}



const WindowPtr GLContext::window() const
{
    return _window;
}




} // namespace cie::gl