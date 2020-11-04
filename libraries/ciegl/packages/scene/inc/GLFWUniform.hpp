#ifndef CIE_GL_GLFW_UNIFORM_HPP
#define CIE_GL_GLFW_UNIFORM_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/AbsUniform.hpp"


namespace cie::gl {


/* --- GLFWUniform --- */

class GLFWUniform : public AbsUniform
{
protected:
    struct Properties
    {
        GLsizei length;
        GLsizei size;
        GLenum  type;
    };

public:
    GLFWUniform( const Shader::Uniform& r_shaderUniform,
                 Size programID );

    virtual void write() = 0;

protected:
    Properties _properties;
};




/* --- GLFWFloatMat4Uniform --- */

class GLFWFloatMat4Uniform final : public GLFWUniform
{
public:
    GLFWFloatMat4Uniform( const Shader::Uniform& r_shaderUniform,
                          Size programID );

    void write() override;
};


} // namespace cie::gl


#endif