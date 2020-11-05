#ifndef CIE_GL_GL_UNIFORM_HPP
#define CIE_GL_GL_UNIFORM_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <glm/glm.hpp>

// --- Utility Includes ---
#include "cieutils/packages/types/inc/IDObject.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shaders/inc/Shader.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::gl {




/* --- GLUniform --- */

/**
 * Interface for loading uniforms into GL.
 */
class GLUniform :
    public Shader::Uniform,
    public utils::IDObject<GLint>
{
protected:
    struct Properties
    {
        GLsizei length;
        GLsizei size;
        GLenum  type;
    };

public:
    GLUniform( const Shader::Uniform& r_shaderUniform,
               Size programID );
    virtual void write() = 0;

    const Properties& properties() const;

protected:
    Properties _properties;
};


using GLUniformPtr = std::shared_ptr<GLUniform>;




/* --- GLUniformPlaceholder --- */

/**
 * Class for storing uniform information from shaders
 * during the cunstruction of a scene. After construction,
 * instances of this class must be replaced by specialized
 * GLUniforms of the correct type by binding specific objects
 * to them.
 */
class GLUniformPlaceholder final : public GLUniform
{
public:
    GLUniformPlaceholder( const Shader::Uniform& r_shaderUniform,
                          Size programID );
    void write() override;
};




/* --- FloatMat4GLUniform --- */

/**
 * 4x4 float matrix uniform used mostly for 3D transformations.
 * Should be bound to the transformation matrix of a camera.
 */
class FloatMat4GLUniform final : public GLUniform
{
public:
    using uniform_type = glm::mat4;

public:
    FloatMat4GLUniform( const Shader::Uniform& r_shaderUniform,
                        Size programID,
                        const uniform_type& r_uniform );

    void write() override;

private:
    const uniform_type& _r_uniform;
};


} // namespace cie::gl

#endif