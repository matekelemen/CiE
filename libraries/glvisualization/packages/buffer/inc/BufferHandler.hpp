#ifndef GLVISUALIZATION_BUFFER_HANDLER_HPP
#define GLVISUALIZATION_BUFFER_HANDLER_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "glvisualization/packages/context/inc/AbsContextClass.hpp"

// --- STL Includes ---
#include <vector>

namespace cie::gl {


class BufferHandler : AbsContextClass
{
public:
    BufferHandler(  GLContext& context,
                    GLuint drawMode );
    virtual ~BufferHandler();
    virtual void terminate() override;

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

#include "glvisualization/packages/buffer/impl/BufferHandler_impl.hpp"

#endif