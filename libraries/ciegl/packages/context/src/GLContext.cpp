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




GLContext::GLContext(   Size versionMajor,
                        Size versionMinor,
                        Size MSAASamples,
                        const std::string& r_logFileName  )   :
    AbsContext( versionMajor,
                versionMinor,
                MSAASamples,
                r_logFileName )
{
    if (!_initialized)
    {
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, this->_version.first );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, this->_version.second );
        glfwWindowHint( GLFW_SAMPLES, MSAASamples );
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



typename GLContext::window_ptr GLContext::newWindow( Size width,
                                                     Size height,
                                                     const std::string& r_name,
                                                     typename GLContext::monitor_ptr p_monitor )
{
    typename GLContext::window_ptr p_newWindow(
        glfwCreateWindow( width, 
                          height,
                          r_name.c_str(),
                          p_monitor.get(),
                          nullptr )
    );

    if ( p_newWindow == nullptr )
    {
        terminate();
        error( "Failed to create window!" );
    }

    log("Open window");
    this->registerWindow( p_newWindow );

    glfwSetFramebufferSizeCallback( p_newWindow.get(), frameBufferResizeCallback );
        
    int checkWidth, checkHeight;
    glfwGetFramebufferSize( p_newWindow.get(), &checkWidth, &checkHeight );
    if ( (size_t)checkWidth!=width || (size_t)checkHeight!=height )
        warn( "Created window is not of the requested size!" );

    return p_newWindow;
}



void GLContext::closeWindow( typename GLContext::window_ptr p_window )
{
    _current = false;

    auto itp_window = std::find( this->windows().begin(),
                                 this->windows().end(),
                                 p_window );

    if ( itp_window != this->windows().end() )
    {
        glfwSetWindowShouldClose( p_window.get(), 1 );
        log( "Close window" );
    }    

    this->deregisterWindow( p_window );
}



void GLContext::makeContextCurrent()
{
    if ( !this->windows().empty() )
    {
        if (!_current)
        {
            glfwMakeContextCurrent( this->windows().begin()->get() );
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

    typename GLContext::window_type* p_window = this->windows().begin()->get();

    // Bind events
    if ( p_window != nullptr )
    {
        glfwSetKeyCallback( p_window, keyCallback );
        glfwSetCursorPosCallback( p_window, cursorCallback );
        glfwSetMouseButtonCallback( p_window, mouseCallback );
        separate();
        log("Start event loop");
    }
    else 
    {
        terminate();
        error( "Attempt to start event loop without an existing window!" );
    }

    // Start event loop
    while( !glfwWindowShouldClose(p_window) )
    {
        if ( !_drawFunction() )
            glfwSetWindowShouldClose(p_window, GLFW_TRUE);

        // Check errors
        GLuint err = glGetError();
        if (err!=0)
            error( "Error drawing! Error code: " + std::to_string(err) );

        glfwPollEvents();

        // Check errors
        err = glGetError();
        if (err!=0)
            error( "Error polling events! Error code: " + std::to_string(err) );
        
        glfwSwapBuffers( p_window );

        // Check errors
        err = glGetError();
        if (err!=0)
            error( "Error swapping buffers! Error code: " + std::to_string(err) );
    }

    log( "End event loop" );
    separate();
    closeWindow( *this->windows().begin() );
}


void GLContext::terminate()
{
    log( "Terminate context" );
    this->closeAllWindows();
    glfwTerminate();
}



WindowPtr GLContext::window()
{
    return this->windows().begin()->get();
}



const WindowPtr GLContext::window() const
{
    return this->windows().begin()->get();
}




} // namespace cie::gl