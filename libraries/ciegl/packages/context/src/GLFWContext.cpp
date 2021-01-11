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


/* --- GLFWContext --- */

GLFWContext::GLFWContext( Size versionMajor,
                          Size versionMinor,
                          Size MSAASamples,
                          const std::filesystem::path& r_logFileName,
                          bool useConsole  )   :
    AbsContext( versionMajor,
                versionMinor,
                MSAASamples,
                r_logFileName,
                useConsole ),
    _isGLADInitialized( false ),
    _isCurrent( false )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto scopedBlock = this->newBlock( "GLFWContext init" );

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMajor );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMinor );
    glfwWindowHint( GLFW_SAMPLES, MSAASamples );
    //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );

    glfwSetErrorCallback( callback_errorPrint );

    if ( !glfwInit() )
        this->error<Exception>( "Failed to initialize GLFW" );
    else
        this->log( "GLFW initialization successful" );

    log( "Open context" );
    
    CIE_END_EXCEPTION_TRACING
}


GLFWContext::~GLFWContext()
{
    glfwTerminate();
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
        // TODO
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




/* --- GLFWContextSingleton --- */

ContextPtr GLFWContextSingleton::_p_context = nullptr;


ContextPtr GLFWContextSingleton::get( Size versionMajor,
                                      Size versionMinor,
                                      Size MSAASamples,
                                      const std::filesystem::path& r_logFilePath,
                                      bool useConsole )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( GLFWContextSingleton::_p_context ) // context is already initialized
    {
        // Check context parameters
        if (
            GLFWContextSingleton::_p_context->version().first != versionMajor
            ||
            GLFWContextSingleton::_p_context->version().second != versionMinor
            ||
            GLFWContextSingleton::_p_context->MSAASamples() != MSAASamples
        )
            GLFWContextSingleton::_p_context->error( "GLFWContext parameter mismatch!" );

        // TODO: Add stream if necessary
    }
    else // initialize the context
    {
        GLFWContextSingleton::_p_context = ContextPtr( new GLFWContext(
            versionMajor,
            versionMinor,
            MSAASamples,
            r_logFilePath,
            useConsole
        ) );
    }

    return GLFWContextSingleton::_p_context;

    CIE_END_EXCEPTION_TRACING
}


ContextPtr GLFWContextSingleton::get()
{
    CIE_BEGIN_EXCEPTION_TRACING

    ContextPtr p_context;

    if ( GLFWContextSingleton::_p_context )
        p_context = GLFWContextSingleton::_p_context;
    else
        p_context = GLFWContextSingleton::get( 4,
                                               5,
                                               0,
                                               OUTPUT_PATH / "graphics.log",
                                               false );

    return p_context;

    CIE_END_EXCEPTION_TRACING
}


ContextPtr GLFWContextSingleton::get( const std::filesystem::path& r_logFilePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_context = GLFWContextSingleton::get();

    // Create log file if it's not currently in use
    auto& r_fileManager = p_context->fileManager();

    auto p_file = r_fileManager.filePtr(
        r_fileManager.newFile( r_logFilePath )
    );
    p_context->addStream( p_file );

    return p_context;

    CIE_END_EXCEPTION_TRACING
}


ContextPtr GLFWContextSingleton::get( bool useConsole )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_context = GLFWContextSingleton::get();

    p_context->useConsole( useConsole );

    return p_context;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl