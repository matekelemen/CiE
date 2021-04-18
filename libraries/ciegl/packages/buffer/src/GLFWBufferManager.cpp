// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/GLFWBufferManager.hpp"
#include "ciegl/packages/buffer/inc/GLFWBuffer.hpp"

namespace cie::gl {


GLFWBufferManager::GLFWBufferManager( GLuint drawMode,
                                      utils::Logger& r_logger ) :
    AbsBufferManager( "GLFWBufferManager", r_logger )
{
    this->setDrawMode( drawMode );
}


GLFWBufferManager::GLFWBufferManager( utils::Logger& r_logger ) :
    GLFWBufferManager( GL_DYNAMIC_DRAW, r_logger )
{
}


VertexBufferPtr GLFWBufferManager::makeVertexBuffer_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    return VertexBufferPtr(
        new GLFWVertexBuffer
    );
    
    CIE_END_EXCEPTION_TRACING
}


ElementBufferPtr GLFWBufferManager::makeElementBuffer_impl()
{
    CIE_BEGIN_EXCEPTION_TRACING

    return ElementBufferPtr(
        new GLFWElementBuffer
    );

    CIE_END_EXCEPTION_TRACING
}


void GLFWBufferManager::bindVertexBuffer_impl( VertexBufferPtr p_buffer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glBindBuffer( GL_ARRAY_BUFFER, p_buffer->getID() );

    CIE_END_EXCEPTION_TRACING
}


void GLFWBufferManager::bindElementBuffer_impl( ElementBufferPtr p_buffer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, p_buffer->getID() );

    CIE_END_EXCEPTION_TRACING
}


void GLFWBufferManager::setDrawMode( GLuint drawMode )
{
    _drawMode = drawMode;
    switch (drawMode)
    {
        case GL_STATIC_DRAW:
            log( "Set GL_STATIC_DRAW");
            break;
        case GL_DYNAMIC_DRAW:
            log( "Set GL_DYNAMIC_DRAW");
            break;
        case GL_STREAM_DRAW:
            log( "Set GL_STREAM_DRAW");
            break;
        default:
            log( "Attempt to set invalid draw mode: " + std::to_string(drawMode), LOG_TYPE_ERROR );
            break;
    }
    
}


} // namespace cie::gl