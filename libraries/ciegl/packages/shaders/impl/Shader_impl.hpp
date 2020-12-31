#ifndef CIE_GL_SHADER_IMPL_HPP
#define CIE_GL_SHADER_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::gl {


template <class ShaderType, class ...Args>
inline ShaderPtr
makeVertexShader( const std::filesystem::path& r_configPath,
                  const std::filesystem::path& r_codePath,
                  Args&&... args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_shader = ShaderPtr(
        new ShaderType( r_configPath,
                        r_codePath,
                        std::forward<Args>(args)... )
    );

    // Vertex shader needs at least one attribute
    if ( p_shader->attributes().empty() )
        CIE_THROW( Exception, "Vertex shader missing attribute(s)!" )

    return p_shader;

    CIE_END_EXCEPTION_TRACING
}


template <class ShaderType, class ...Args>
inline ShaderPtr
makeGeometryShader( const std::filesystem::path& r_configPath,
                    const std::filesystem::path& r_codePath,
                    Args&&... args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_shader = ShaderPtr(
        new ShaderType( r_configPath,
                        r_codePath,
                        std::forward<Args>(args)... )
    );

    // Geometry shader has no configuration requirements
    return p_shader;

    CIE_END_EXCEPTION_TRACING
}


template <class ShaderType, class ...Args>
inline ShaderPtr
makeFragmentShader( const std::filesystem::path& r_configPath,
                    const std::filesystem::path& r_codePath,
                    Args&&... args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_shader = ShaderPtr(
        new ShaderType( r_configPath,
                        r_codePath,
                        std::forward<Args>(args)... )
    );

    // Fragment shader needs at least one output
    if ( p_shader->outputs().empty() )
        CIE_THROW( Exception, "Fragment shader missing output(s)!" )

    return p_shader;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl


#endif