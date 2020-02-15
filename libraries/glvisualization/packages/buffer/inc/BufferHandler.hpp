#ifndef GLVISUALIZATION_BUFFER_HANDLER_HPP
#define GLVISUALIZATION_BUFFER_HANDLER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "linalgtypes.hpp"
#include "../../context/inc/AbsContextClass.hpp"

#include <vector>

namespace cie {
namespace gl {


class BufferHandler : AbsContextClass
{
public:
    BufferHandler(  GLContext& context,
                    GLuint drawMode,
                    GLuint numberOfBuffers=2 );
    [[nodiscard]] GLuint createBuffer();

    void bindVertexBuffer( GLuint bufferID );
    void bindElementBuffer( GLuint bufferID );

    template <template<class ...> class ContainerType, class DataType, class ...Args>
    void writeToActiveBuffer(   GLenum target,
                                const ContainerType<DataType, Args...>& data );

    void setDrawMode( GLuint drawMode );

private:
    std::vector<GLuint>     _buffers;
    GLuint                  _drawMode;
};


}
}

#include "BufferHandler_impl.hpp"

#endif