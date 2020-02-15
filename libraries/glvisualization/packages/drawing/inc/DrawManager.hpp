#ifndef GLVISUALIZATION_DRAW_MANAGER_HPP
#define GLVISUALIZATION_DRAW_MANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../context/inc/AbsContextClass.hpp"
#include "../../buffer/inc/BufferHandler.hpp"
#include "../../shaders/inc/ShaderManager.hpp"
#include <string>

namespace cie {
namespace gl {


class DrawManager : public AbsContextClass
{
public:
    DrawManager( GLContext& context );
    virtual void initialize();
    virtual void draw();

    void compileShaders();
    void makeProgram();

    BufferHandler& buffers();
    const BufferHandler& buffers() const;
    ShaderManager& shaderCode();
    const std::vector<GLuint>& shaders() const;

protected:
    BufferHandler       _buffers;
    ShaderManager       _shaderCode;
    std::vector<GLuint> _shaders;
    GLuint              _programID;
};


}
}

#endif