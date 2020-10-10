// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/defaultCallback.hpp"

// --- STL Includes ---
#include <string>
#include <iostream>

namespace cie::gl {


/* --- Default callbacks --- */

DrawFunction makeEmptyDrawFunction( GLFWContext& )
{
    auto function = [&]() -> bool { return true; };
    return function;
}


/* --- Static initialization --- */

bool GLFWContext::_isGLFWInitialized    = false;
bool GLFWContext::_active               = false;
bool GLFWContext::_isGLADInitialized    = false;



/* --- GLFWContext --- */

GLFWContext::GLFWContext( Size versionMajor,
                          Size versionMinor,
                          Size MSAASamples,
                          const std::string& r_logFileName  )   :
    AbsContext( versionMajor,
                versionMinor,
                MSAASamples,
                r_logFileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if (!_isGLFWInitialized)
    {
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, this->_version.first );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, this->_version.second );
        glfwWindowHint( GLFW_SAMPLES, MSAASamples );
        //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );

        // Bind callbacks and event loops
        glfwSetErrorCallback( callback_errorPrint );

        if ( !glfwInit() )
            error<Exception>( "Failed to initialize GLFW" );

        _isGLFWInitialized = true;
        log( "Open context" );
    }
    else
        warn( "Attempt to reinitialize the context" );

    CIE_END_EXCEPTION_TRACING
}



GLFWContext::~GLFWContext()
{
    log( "Terminate context" );
    this->closeAllWindows();
    glfwTerminate();
    log( "Destroy context" );
}



typename GLFWContext::window_ptr GLFWContext::newWindow( Size width,
                                                         Size height,
                                                         const std::string& r_name,
                                                         typename GLFWContext::monitor_ptr p_monitor )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto scopedBlock = this->newBlock( "open new window" );

    // Create new window
    typename GLFWContext::window_ptr p_newWindow(
        glfwCreateWindow( width, 
                          height,
                          r_name.c_str(),
                          p_monitor.get(),
                          nullptr )
    );

    // Check whether opening a new window was successful
    if ( p_newWindow == nullptr )
        error<Exception>( "Failed to create window!" );

    // Register
    log("Open window");
    this->registerWindow( p_newWindow );
    glfwSetFramebufferSizeCallback( p_newWindow.get(), frameBufferResizeCallback );

    // Check whether the new window has the requested parameters
    int checkWidth, checkHeight;
    glfwGetFramebufferSize( p_newWindow.get(), &checkWidth, &checkHeight );
    if ( (size_t)checkWidth!=width || (size_t)checkHeight!=height )
        warn( "Created window is not of the requested size!" );

    return p_newWindow;

    CIE_END_EXCEPTION_TRACING
}



void GLFWContext::focusWindow( typename GLFWContext::window_ptr p_window )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( p_window != nullptr )
    {
        glfwMakeContextCurrent( p_window.get() );
        log( "Focus on window " );
        initializeGLADIfNecessary();
        glDebugMessageCallback( messageCallback, this );
    }
    else
        error<Exception>( "Focus on non-existent window!" );

    CIE_END_EXCEPTION_TRACING
}



void GLFWContext::closeWindow( typename GLFWContext::window_ptr p_window )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->deregisterWindow( p_window );
    glfwSetWindowShouldClose( p_window.get(), 1 );
    log( "Close window" );

    CIE_END_EXCEPTION_TRACING
}



void GLFWContext::startEventLoop( DrawFunctionFactory eventLoopGenerator,
                                  KeyCallbackFunction keyCallback,
                                  CursorCallbackFunction cursorCallback,
                                  MouseCallbackFunction mouseCallback )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Bind loop
    _drawFunction = eventLoopGenerator( *this );

    typename GLFWContext::window_ptr p_window = nullptr;
    if ( !this->windows().empty() )
        p_window = *this->windows().begin();

    auto p_rawWindow = p_window.get();

    // Bind events
    if ( p_rawWindow )
    {
        glfwSetKeyCallback( p_rawWindow, keyCallback );
        glfwSetCursorPosCallback( p_rawWindow, cursorCallback );
        glfwSetMouseButtonCallback( p_rawWindow, mouseCallback );
    }
    else 
        error<Exception>( "Attempt to start event loop without an existing window!" );

    // Start event loop
    {
        auto scopedBlock = this->newBlock( "Event Loop" );

        while( !glfwWindowShouldClose(p_rawWindow) )
        {
            if ( !_drawFunction() )
                this->closeWindow( p_window );

            // Check for drawing errors
            GLuint err = glGetError();
            if (err!=0)
                error<Exception>( "Error drawing! Error code: " + std::to_string(err) );

            glfwPollEvents();

            // Check for polling errors
            err = glGetError();
            if (err!=0)
                error<Exception>( "Error polling events! Error code: " + std::to_string(err) );
            
            glfwSwapBuffers( p_rawWindow );

            // Check for buffer errors
            err = glGetError();
            if (err!=0)
                error<Exception>( "Error swapping buffers! Error code: " + std::to_string(err) );
        }
    }

    CIE_END_EXCEPTION_TRACING
}



WindowPtr GLFWContext::window()
{
    CIE_BEGIN_EXCEPTION_TRACING

    return this->windows().begin()->get();

    CIE_END_EXCEPTION_TRACING
}



const WindowPtr GLFWContext::window() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return this->windows().begin()->get();

    CIE_END_EXCEPTION_TRACING
}



void GLFWContext::initializeGLADIfNecessary()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( _isGLADInitialized )
        return;

    if ((!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
        error<Exception>( "Failed to initialize GLAD" );
    else
        log("Initialize GLAD");

    _isGLADInitialized = true;

    CIE_END_EXCEPTION_TRACING
}



} // namespace cie::gl