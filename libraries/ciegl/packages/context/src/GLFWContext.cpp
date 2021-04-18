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
                          utils::LoggerPtr p_logger  )   :
    AbsContext( versionMajor,
                versionMinor,
                MSAASamples,
                p_logger ),
    _isGLADInitialized( false ),
    _isCurrent( false )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto scopedBlock = _p_logger->newBlock( "GLFWContext init" );

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMajor );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, versionMinor );
    glfwWindowHint( GLFW_SAMPLES, MSAASamples );
    //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );

    glfwSetErrorCallback( callback_errorPrint );

    if ( !glfwInit() )
        _p_logger->error<Exception>( "Failed to initialize GLFW" );
    else
        _p_logger->log( "GLFW initialization successful" );

    _p_logger->log( "Open context" );
    
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
                        *_p_logger)
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
        _p_logger->log( "Focus on window " );
    }
    else
        _p_logger->error<Exception>( "Focus on non-existent window!" );

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
        _p_logger->error<Exception>( "Failed to initialize GLAD" );
    else
        _p_logger->log("Initialize GLAD");

    _isGLADInitialized = true;

    CIE_END_EXCEPTION_TRACING
}




/* --- GLFWContextSingleton --- */

ContextPtr GLFWContextSingleton::_p_context = nullptr;


ContextPtr GLFWContextSingleton::get( Size versionMajor,
                                      Size versionMinor,
                                      Size MSAASamples,
                                      utils::LoggerPtr p_logger )
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
            GLFWContextSingleton::_p_context->logger().error( "GLFWContext parameter mismatch!" );

        // TODO: Add stream if necessary
    }
    else // initialize the context
    {
        GLFWContextSingleton::_p_context = ContextPtr( new GLFWContext(
            versionMajor,
            versionMinor,
            MSAASamples,
            p_logger
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
                                               utils::LoggerSingleton::getPtr() );

    return p_context;

    CIE_END_EXCEPTION_TRACING
}


ContextPtr GLFWContextSingleton::get( utils::LoggerPtr p_logger )
{
    CIE_BEGIN_EXCEPTION_TRACING

    ContextPtr p_context;

    if ( !GLFWContextSingleton::_p_context )
        p_context = GLFWContextSingleton::get( 4, 5, 0, p_logger );
    else
    {
        if ( &p_context->logger() != &*p_logger )
            CIE_THROW( Exception, "Cannot assign new logger to GLFWContextSingleton" )
    
        p_context = GLFWContextSingleton::_p_context;
    }

    return p_context;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl