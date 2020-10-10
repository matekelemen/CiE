#ifndef CIE_GL_GLFW_BUFFER_IMPL_HPP
#define CIE_GL_GLFW_BUFFER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::gl {


template <class DataType>
GLFWBuffer<DataType>::GLFWBuffer( Size id ) :
    AbsBuffer<DataType>(id),
    _drawMode( GL_STATIC_DRAW )
{
}


template <class DataType>
void
GLFWBuffer<DataType>::write( const typename GLFWBuffer::data_container_type& r_dataContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glBufferData(
        GLuint( this->_id ),
        r_dataContainer.size()*sizeof(typename GLFWBuffer::data_type),
        &*r_dataContainer.begin(),
        _drawMode 
    );

    CIE_END_EXCEPTION_TRACING
}


template <class DataType>
void
GLFWBuffer<DataType>::setDrawMode( Size drawMode )
{
    _drawMode = drawMode;
}


template <class DataType>
const GLuint
GLFWBuffer<DataType>::getDrawMode() const
{
    return _drawMode;
}


} // namespace cie::gl

#endif