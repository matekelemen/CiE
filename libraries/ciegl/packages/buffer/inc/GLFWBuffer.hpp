#ifndef CIE_GL_GLFW_BUFFER_HPP
#define CIE_GL_GLFW_BUFFER_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AbsBuffer.hpp"


namespace cie::gl {


template <class DataType, class DataContainer = std::vector<DataType>>
requires concepts::ClassContainer<DataContainer,DataType>
class GLFWBuffer : public AbsBuffer<DataType,DataContainer>
{
public:
    GLFWBuffer();
    ~GLFWBuffer();

    virtual void reserve( Size byteCount ) override;

    virtual void write( const typename GLFWBuffer<DataType,DataContainer>::data_container_type& r_dataContainer ) override;

    virtual void write( Size begin, const typename GLFWBuffer<DataType,DataContainer>::data_container_type& r_dataContainer ) override;

    void setDrawMode( Size drawMode );
    const GLuint getDrawMode() const;

protected:
    GLuint _drawMode;
};


using GLFWVertexBuffer  = GLFWBuffer<GLfloat,AttributeContainer>;
using GLFWElementBuffer = GLFWBuffer<GLuint>;


} // namespace cie::gl

#include "ciegl/packages/buffer/impl/GLFWBuffer_impl.hpp"

#endif