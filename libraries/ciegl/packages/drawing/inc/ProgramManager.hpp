#ifndef CIEGL_PROGRAM_MANAGER_HPP
#define CIEGL_PROGRAM_MANAGER_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsContextClass.hpp"
#include "ciegl/packages/buffer/inc/BufferManager.hpp"

// --- Standard Includes ---
#include <string>

namespace cie::gl {


class ProgramManager : public AbsContextClass
{
public:
    ProgramManager(     GLFWContext& context,
                        const std::string& className = "ProgramManager" );
    virtual ~ProgramManager();

    virtual void initialize();

    virtual void compileShaders();
    virtual void makeProgram();

    BufferManager& buffers();
    const BufferManager& buffers() const;
    const std::vector<GLuint>& shaders() const;

protected:
    BufferManager       _buffers;
    std::vector<GLuint> _shaders;

    GLuint              _programID;
    GLuint              _vaoID;
};


} // namespace cie::gl

#endif