// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/context/inc/defaultCallback.hpp"

// --- STL Includes ---
#include <string>
#include <iostream>

namespace cie::gl {


namespace detail {
GLFWRAII::GLFWRAII( Size versionMajor,
                    Size versionMinor,
                    Size MSAASamples,
                    utils::Logger& r_logger ) :
    _versionMajor( versionMajor ),
    _versionMinor( versionMinor ),
    _MSAASamples( MSAASamples )
{
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMajor );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMinor );
    glfwWindowHint( GLFW_SAMPLES, MSAASamples );
    //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );

    glfwSetErrorCallback( callback_errorPrint );

    if ( !glfwInit() )
        r_logger.error<Exception>( "Failed to initialize GLFW" );
    else
        r_logger.log( "GLFW initialization successful" );
}

GLFWRAII::~GLFWRAII()
{
    glfwTerminate();
}
} // namespace detail


/* --- Default callbacks --- */

DrawFunction makeEmptyDrawFunction( GLFWContext& )
{
    auto function = [&]() -> bool { return true; };
    return function;
}


/* --- STATIC INITIALIZATION --- */
bool GLFWContext::_isGLADInitialized = false;
bool GLFWContext::_isCurrent         = false;


/* --- GLFWContext --- */

GLFWContext::GLFWContext( Size versionMajor,
                          Size versionMinor,
                          Size MSAASamples,
                          const std::string& r_logFileName,
                          bool useConsole  )   :
    AbsContext( versionMajor,
                versionMinor,
                MSAASamples,
                r_logFileName,
                useConsole )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto scopedBlock = this->newBlock( "GLFWContext init" );

    static detail::GLFWRAII glfwInitializer( versionMajor,
                                             versionMinor,
                                             MSAASamples,
                                             *this );

    // Check version a sampling
    // --> once set up with a particular set of parameters,
    //     other contexts must have the identical ones
    if ( this->_version.first != glfwInitializer._versionMajor
         || this->_version.second != glfwInitializer._versionMinor
         || this->_MSAASamples != glfwInitializer._MSAASamples )
        CIE_THROW( Exception, "GLFWContext initialization mismatch!" )

    log( "Open context" );
    
    CIE_END_EXCEPTION_TRACING
}


GLFWContext::~GLFWContext()
{
    this->closeAllWindows();
}



WindowPtr GLFWContext::newWindow_impl( Size width,
                                       Size height,
                                       const std::string& r_name )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_newWindow = WindowPtr(
        new GLFWWindow( 0,
                        r_name,
                        width,
                        height,
                        *this )
    );

    if ( !this->_isCurrent )
        {
            glfwMakeContextCurrent( detail::getGLFWwindow(p_newWindow) );
            this->log( "make context current" );
            initializeGLADIfNecessary();
            this->_isCurrent = true;
            glDebugMessageCallback( messageCallback, this );
        }

    return p_newWindow;

    CIE_END_EXCEPTION_TRACING
}



void GLFWContext::focusWindow( WindowPtr p_window )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( p_window != nullptr )
    {
        log( "Focus on window " );
    }
    else
        error<Exception>( "Focus on non-existent window!" );

    CIE_END_EXCEPTION_TRACING
}



void GLFWContext::closeWindow_impl( WindowPtr p_window )
{
    CIE_BEGIN_EXCEPTION_TRACING
    glfwSetWindowShouldClose( detail::getGLFWwindow(p_window), 1 );
    CIE_END_EXCEPTION_TRACING
}



void GLFWContext::startEventLoop( DrawFunctionFactory eventLoopGenerator )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Bind loop
    _drawFunction = eventLoopGenerator( *this );

    WindowPtr p_window = nullptr;
    if ( !this->windows().empty() )
        p_window = this->window();

    auto p_rawWindow = detail::getGLFWwindow(p_window);

    if ( !p_rawWindow )
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

    return *this->windows().begin();

    CIE_END_EXCEPTION_TRACING
}



const WindowPtr GLFWContext::window() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return *this->windows().begin();

    CIE_END_EXCEPTION_TRACING
}



void GLFWContext::initializeGLADIfNecessary()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( _isGLADInitialized )
        return;

    if ( (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress )) )
        error<Exception>( "Failed to initialize GLAD" );
    else
        log("Initialize GLAD");

    _isGLADInitialized = true;

    CIE_END_EXCEPTION_TRACING
}



} // namespace cie::gl