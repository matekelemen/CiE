#ifndef CIE_GL_ABS_UNIFORM_HPP
#define CIE_GL_ABS_UNIFORM_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/IDObject.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shaders/inc/Shader.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::gl {


class AbsUniform :
    public Shader::Uniform,
    public utils::IDObject<Size>
{
public:
    AbsUniform( const Shader::Uniform& r_shaderUniform );
    virtual void write() = 0;
};



class UniformPlaceholder final : public AbsUniform
{
public:
    UniformPlaceholder( const Shader::Uniform& r_shaderUniform );
    void write() override;
};



using UniformPtr = std::shared_ptr<AbsUniform>;


} // namespace cie::gl

#endif