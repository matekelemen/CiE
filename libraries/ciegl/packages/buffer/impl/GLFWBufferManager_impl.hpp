#ifndef CIEGL_GLFW_BUFFER_MANAGER_IMPL_HPP
#define CIEGL_GLFW_BUFFER_MANAGER_IMPL_HPP

namespace cie::gl {


template <template<class ...> class ContainerType, class DataType, class ...Args>
void GLFWBufferManager::writeToActiveBuffer( GLenum target, const ContainerType<DataType, Args...>& data )
{
    glBufferData( target, data.size()*sizeof(DataType), &(data[0]), _drawMode );
}


} // namespace cie::gl

#endif