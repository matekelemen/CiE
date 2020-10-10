#ifndef CIE_GL_GLFW_BUFFER_HPP
#define CIE_GL_GLFW_BUFFER_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AbsBuffer.hpp"


namespace cie::gl {


template <class DataType>
class GLFWBuffer : public AbsBuffer<DataType>
{
public:
    GLFWBuffer( Size id );

    void write( const typename GLFWBuffer::data_container_type& r_dataContainer ) override;

    void setDrawMode( Size drawMode );
    const GLuint getDrawMode() const;

protected:
    GLuint _drawMode;
};


class GLFWVertexBuffer final : public GLFWBuffer<GLfloat>
{
public:
    GLFWVertexBuffer( Size id );
    void write( const typename GLFWVertexBuffer::data_container_type& r_dataContainer ) override;
};


class GLFWElementBuffer final : public GLFWBuffer<unsigned int>
{
public:
    GLFWElementBuffer( Size id );
    void write( const typename GLFWElementBuffer::data_container_type& r_dataContainer ) override;
};


} // namespace cie::gl

#include "ciegl/packages/buffer/impl/GLFWBuffer_impl.hpp"

#endif