#ifndef CIE_GL_GLFW_SHADER_HPP
#define CIE_GL_GLFW_SHADER_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "ciegl/packages/shaders/inc/Shader.hpp"


namespace cie::gl {


class GLFWShader : public Shader
{
public:
    /**
     * Choices for shader types:
     *  GL_VERTEX_SHADER
     *  GL_GEOMETRY_SHADER
     *  GL_FRAGMENT_SHADER
     */
    GLFWShader( const std::string& r_configPath,
                const std::string& r_codePath,
                Size shaderType );
    ~GLFWShader();
};


struct GLFWVertexShader : public GLFWShader
{
    GLFWVertexShader( const std::string& r_configPath,
                      const std::string& r_codePath ) :
        GLFWShader( r_configPath,
                    r_codePath,
                    GL_VERTEX_SHADER )
    {}
};


struct GLFWGeometryShader : public GLFWShader
{
    GLFWGeometryShader( const std::string& r_configPath,
                        const std::string& r_codePath ) :
        GLFWShader( r_configPath,
                    r_codePath,
                    GL_GEOMETRY_SHADER )
    {}
};


struct GLFWFragmentShader : public GLFWShader
{
    GLFWFragmentShader( const std::string& r_configPath,
                        const std::string& r_codePath ) :
        GLFWShader( r_configPath,
                    r_codePath,
                    GL_FRAGMENT_SHADER )
    {}
};


} // namespace cie::gl


#endif