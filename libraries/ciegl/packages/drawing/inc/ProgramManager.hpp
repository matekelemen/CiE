#ifndef CIEGL_PROGRAM_MANAGER_HPP
#define CIEGL_PROGRAM_MANAGER_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsContextClass.hpp"
#include "ciegl/packages/buffer/inc/BufferHandler.hpp"

// --- Standard Includes ---
#include <string>

namespace cie::gl {


class ProgramManager : public AbsContextClass
{
public:
    ProgramManager(     GLContext& context,
                        const std::string& className = "ProgramManager" );
    virtual ~ProgramManager();

    virtual void initialize();
    virtual void terminate() override;

    virtual void compileShaders();
    virtual void makeProgram();

    BufferHandler& buffers();
    const BufferHandler& buffers() const;
    const std::vector<GLuint>& shaders() const;

protected:
    BufferHandler       _buffers;
    std::vector<GLuint> _shaders;

    GLuint              _programID;
    GLuint              _vaoID;
};


} // namespace cie::gl

#endif