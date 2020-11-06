#ifndef CIE_GL_ABS_BUFFER_MANAGER_IMPL_HPP
#define CIE_GL_ABS_BUFFER_MANAGER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"


namespace cie::gl {


inline AbsBufferManager& AbsBufferManager::addBuffer( VertexBufferPtr p_buffer )
{
    this->registerBuffer( p_buffer, this->_vertexBuffers );
    return *this;
}


inline AbsBufferManager& AbsBufferManager::addBuffer( ElementBufferPtr p_buffer )
{
    this->registerBuffer( p_buffer, this->_elementBuffers );
    return *this;
}


template <class BufferPtr, class BufferContainer>
requires concepts::ClassContainer<BufferContainer,BufferPtr>
inline void
AbsBufferManager::registerBuffer( BufferPtr p_buffer, BufferContainer& r_bufferContainer )
{
    r_bufferContainer.push_back( p_buffer );
}


template <class ContainerType>
inline AbsBufferManager&
AbsBufferManager::writeToBoundVertexBuffer( const ContainerType& r_data )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( this->_p_boundVertexBuffer )
    this->_p_boundVertexBuffer->write( r_data );

    return *this;

    CIE_END_EXCEPTION_TRACING
}


template <class ContainerType>
inline AbsBufferManager&
AbsBufferManager::writeToBoundElementBuffer( const ContainerType& r_data )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( this->_p_boundElementBuffer )
    this->_p_boundElementBuffer->write( r_data );

    return *this;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl


#endif