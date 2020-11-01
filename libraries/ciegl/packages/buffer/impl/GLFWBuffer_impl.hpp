#ifndef CIE_GL_GLFW_BUFFER_IMPL_HPP
#define CIE_GL_GLFW_BUFFER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <limits>


namespace cie::gl {


template <class DataType>
GLFWBuffer<DataType>::GLFWBuffer() :
    AbsBuffer<DataType>( std::numeric_limits<Size>().max() ),
    _drawMode( GL_STATIC_DRAW )
{
    CIE_BEGIN_EXCEPTION_TRACING

    GLuint tempID;
    glGenBuffers( 1, &tempID );
    this->setID( tempID );

    CIE_END_EXCEPTION_TRACING
}


template <class DataType>
GLFWBuffer<DataType>::~GLFWBuffer()
{
    GLuint id = this->getID();
    glDeleteBuffers( 1, &id );
}


template <class DataType>
void
GLFWBuffer<DataType>::write( const typename GLFWBuffer<DataType>::data_container_type& r_dataContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glNamedBufferData(
        GLuint( this->_id ),
        r_dataContainer.size() * sizeof(typename GLFWBuffer<DataType>::data_type),
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