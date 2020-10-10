#ifndef CIEGL_BUFFER_HANDLER_HPP
#define CIEGL_BUFFER_HANDLER_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsContextClass.hpp"
#include "ciegl/packages/buffer/inc/AbsBuffer.hpp"

// --- STL Includes ---
#include <vector>

namespace cie::gl {


class BufferHandler : AbsContextClass
{
public:
    BufferHandler(  GLFWContext& context,
                    GLuint drawMode );
    virtual ~BufferHandler();

    [[nodiscard]] GLuint createBuffer();

    void bindVertexBuffer( GLuint bufferID );
    void bindElementBuffer( GLuint bufferID );

    template <template<class ...> class ContainerType, class DataType, class ...Args>
    void writeToActiveBuffer(   GLenum target,
                                const ContainerType<DataType, Args...>& data );

    void setDrawMode( GLuint drawMode );

protected:
    std::vector<GLuint>     _buffers;

private:
    GLuint                  _drawMode;
};


} // namespace cie::gl

#include "ciegl/packages/buffer/impl/BufferHandler_impl.hpp"

#endif