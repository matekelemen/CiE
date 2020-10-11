#ifndef CIE_GL_ABS_BUFFER_MANAGER_IMPL_HPP
#define CIE_GL_ABS_BUFFER_MANAGER_IMPL_HPP


namespace cie::gl {


template <class BufferPtr, class BufferContainer>
requires concepts::ClassContainer<BufferContainer,BufferPtr>
inline void
AbsBufferManager::registerBuffer( BufferPtr p_buffer, BufferContainer& r_bufferContainer )
{
    r_bufferContainer.push_back( p_buffer );
}


} // namespace cie::gl


#endif