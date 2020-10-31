// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/GLFWBuffer.hpp"


namespace cie::gl {


GLFWVertexBuffer::GLFWVertexBuffer() :
    GLFWBuffer<typename GLFWVertexBuffer::data_type>()
{
}


void GLFWVertexBuffer::write( const typename GLFWVertexBuffer::data_container_type& r_dataContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glBufferData(
        GL_ARRAY_BUFFER,
        r_dataContainer.size()*sizeof(typename GLFWVertexBuffer::data_type),
        &*r_dataContainer.begin(),
        this->_drawMode 
    );

    CIE_END_EXCEPTION_TRACING
}


GLFWElementBuffer::GLFWElementBuffer() :
    GLFWBuffer<typename GLFWElementBuffer::data_type>()
{
}


void GLFWElementBuffer::write( const typename GLFWElementBuffer::data_container_type& r_dataContainer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        r_dataContainer.size()*sizeof(typename GLFWElementBuffer::data_type),
        &*r_dataContainer.begin(),
        _drawMode 
    );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl