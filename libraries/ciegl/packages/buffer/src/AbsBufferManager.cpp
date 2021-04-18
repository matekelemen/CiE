// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AbsBufferManager.hpp"


namespace cie::gl {


AbsBufferManager::AbsBufferManager( const std::string& r_name,
                                    utils::Logger& r_logger ) :
    utils::Loggee( r_logger, r_name )
{
}


VertexBufferPtr AbsBufferManager::makeVertexBuffer()
{
    CIE_BEGIN_EXCEPTION_TRACING

    VertexBufferPtr p_buffer = makeVertexBuffer_impl();
    registerBuffer( p_buffer, _vertexBuffers );

    this->logID( "create vertex buffer", p_buffer->getID() );

    return p_buffer;

    CIE_END_EXCEPTION_TRACING
}


ElementBufferPtr AbsBufferManager::makeElementBuffer()
{
    CIE_BEGIN_EXCEPTION_TRACING

    ElementBufferPtr p_buffer = makeElementBuffer_impl();
    registerBuffer( p_buffer, _elementBuffers );

    this->logID( "create element buffer", p_buffer->getID() );

    return p_buffer;

    CIE_END_EXCEPTION_TRACING
}


AbsBufferManager& AbsBufferManager::bindVertexBuffer( VertexBufferPtr p_buffer,
                                                      bool log )
{
    CIE_BEGIN_EXCEPTION_TRACING

    bindVertexBuffer_impl( p_buffer );
    _p_boundVertexBuffer = p_buffer;

    if ( log )
        this->logID( "Bind vertex buffer", p_buffer->getID() );

    return *this;

    CIE_END_EXCEPTION_TRACING
}


AbsBufferManager& AbsBufferManager::bindElementBuffer( ElementBufferPtr p_buffer,
                                                       bool log )
{
    CIE_BEGIN_EXCEPTION_TRACING

    bindElementBuffer_impl( p_buffer );
    _p_boundElementBuffer = p_buffer;

    if ( log )
        this->logID( "Bind element buffer", p_buffer->getID() );

    return *this;

    CIE_END_EXCEPTION_TRACING
}


bool AbsBufferManager::hasBoundVertexBuffer() const
{
    return _p_boundVertexBuffer ? true : false;
}


bool AbsBufferManager::hasBoundElementBuffer() const
{
    return _p_boundElementBuffer ? true : false;
}


const VertexBufferPtr& AbsBufferManager::boundVertexBuffer() const
{
    return this->_p_boundVertexBuffer;
}


const ElementBufferPtr& AbsBufferManager::boundElementBuffer() const
{
    return this->_p_boundElementBuffer;
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