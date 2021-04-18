#ifndef CIEGL_GLFW_BUFFER_MANAGER_HPP
#define CIEGL_GLFW_BUFFER_MANAGER_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/AbsBufferManager.hpp"
#include "ciegl/packages/buffer/inc/AbsBuffer.hpp"

// --- STL Includes ---
#include <vector>

namespace cie::gl {


class GLFWBufferManager : public AbsBufferManager
{
public:
    GLFWBufferManager( GLuint drawMode,
                       utils::Logger& r_logger = utils::LoggerSingleton::get() );
    GLFWBufferManager( utils::Logger& r_logger = utils::LoggerSingleton::get() );

    void setDrawMode( GLuint drawMode );

protected:
    virtual VertexBufferPtr makeVertexBuffer_impl() override;
    virtual ElementBufferPtr makeElementBuffer_impl() override;
    virtual void bindVertexBuffer_impl( VertexBufferPtr p_buffer ) override;
    virtual void bindElementBuffer_impl( ElementBufferPtr p_buffer ) override;

private:
    GLuint _drawMode;
};


} // namespace cie::gl

#endif