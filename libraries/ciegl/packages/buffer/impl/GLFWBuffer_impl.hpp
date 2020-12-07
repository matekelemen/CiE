#ifndef CIE_GL_GLFW_BUFFER_IMPL_HPP
#define CIE_GL_GLFW_BUFFER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <limits>


namespace cie::gl {


template <class DataType, class DataContainer>
GLFWBuffer<DataType,DataContainer>::GLFWBuffer() :
    AbsBuffer<DataType,DataContainer>( std::numeric_limits<Size>().max() ),
    _drawMode( GL_STATIC_DRAW )
{
    CIE_BEGIN_EXCEPTION_TRACING

    GLuint tempID;
    glGenBuffers( 1, &tempID );
    this->setID( tempID );

    CIE_END_EXCEPTION_TRACING
}


template <class DataType, class DataContainer>
GLFWBuffer<DataType,DataContainer>::~GLFWBuffer()
{
    GLuint id = this->getID();
    glDeleteBuffers( 1, &id );
}


template <class DataType, class DataContainer>
inline void
GLFWBuffer<DataType,DataContainer>::reserve( Size byteCount )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glNamedBufferData( GLuint( this->_id ),
                       byteCount,
                       NULL,
                       _drawMode );

    CIE_END_EXCEPTION_TRACING
}


template <class DataType, class DataContainer>
inline void
GLFWBuffer<DataType,DataContainer>::write( const typename GLFWBuffer<DataType,DataContainer>::data_container_type& r_dataContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glNamedBufferData(
        GLuint( this->_id ),
        r_dataContainer.size() * sizeof(typename GLFWBuffer<DataType,DataContainer>::data_type),
        &*r_dataContainer.begin(),
        _drawMode
    );

    CIE_END_EXCEPTION_TRACING
}


template <class DataType, class DataContainer>
inline void
GLFWBuffer<DataType,DataContainer>::write( Size begin, const typename GLFWBuffer<DataType,DataContainer>::data_container_type& r_dataContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glNamedBufferSubData(
        GLuint( this->_id ),
        begin,
        r_dataContainer.size() * sizeof( typename GLFWBuffer<DataType,DataContainer>::data_type ),
        &*r_dataContainer.begin()
    );

    CIE_END_EXCEPTION_TRACING
}


template <class DataType, class DataContainer>
void
GLFWBuffer<DataType,DataContainer>::setDrawMode( Size drawMode )
{
    _drawMode = drawMode;
}


template <class DataType, class DataContainer>
const GLuint
GLFWBuffer<DataType,DataContainer>::getDrawMode() const
{
    return _drawMode;
}


} // namespace cie::gl

#endif