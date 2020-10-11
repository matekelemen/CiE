// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AbsBufferManager.hpp"


namespace cie::gl {


VertexBufferPtr AbsBufferManager::makeVertexBuffer()
{
    CIE_BEGIN_EXCEPTION_TRACING

    VertexBufferPtr p_vertexBuffer = makeVertexBuffer_impl();
    registerBuffer( p_vertexBuffer, _vertexBuffers );

    return p_vertexBuffer;

    CIE_END_EXCEPTION_TRACING
}


ElementBufferPtr AbsBufferManager::makeElementBuffer()
{
    CIE_BEGIN_EXCEPTION_TRACING

    ElementBufferPtr p_elementBuffer = makeElementBuffer_impl();
    registerBuffer( p_elementBuffer, _elementBuffers );

    return p_elementBuffer;

    CIE_END_EXCEPTION_TRACING
}


void AbsBufferManager::bindVertexBuffer( VertexBufferPtr p_vertexBuffer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    bindVertexBuffer_impl( p_vertexBuffer );
    _p_boundVertexBuffer = p_vertexBuffer;

    CIE_END_EXCEPTION_TRACING
}


void AbsBufferManager::bindElementBuffer( ElementBufferPtr p_elementBuffer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    bindElementBuffer_impl( p_elementBuffer );
    _p_boundElementBuffer = p_elementBuffer;

    CIE_END_EXCEPTION_TRACING
}


const typename AbsBufferManager::vertex_buffer_container& AbsBufferManager::vertexBuffers() const
{
    return _vertexBuffers;
}


const typename AbsBufferManager::element_buffer_container& AbsBufferManager::elementBuffers() const
{
    return _elementBuffers;
}


} // namespace cie::gl