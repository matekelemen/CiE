// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"

// --- STL Includes ---
#include <sstream>


namespace cie::gl {


GLFWShader::GLFWShader( const std::filesystem::path& r_configPath,
                        const std::filesystem::path& r_codePath,
                        Size shaderType ) :
    Shader( r_configPath,
            r_codePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( shaderType != Size(GL_VERTEX_SHADER)
         && shaderType != Size(GL_GEOMETRY_SHADER)
         && shaderType != Size(GL_FRAGMENT_SHADER) )
        CIE_THROW( Exception, "Unrecognized glfw shader enum: " + std::to_string(shaderType) )

    // Register new shader
    GLuint id = glCreateShader( shaderType );
    this->setID( id );

    // Feed in the source code
    auto source = this->_source.c_str();
    glShaderSource( id, 1, &source, NULL );

    // Compile and heck errors
    glCompileShader( id );

    GLint compileStatus;
    glGetShaderiv( id, GL_COMPILE_STATUS, &compileStatus );

    if ( compileStatus != GL_TRUE )
    {
        GLchar buffer[512];
        glGetShaderInfoLog( id, sizeof(buffer)/sizeof(GLchar), NULL, buffer );

        std::stringstream message;
        message << "Shader compilation failed | ID_" << id << std::endl
                << buffer;
        CIE_THROW( Exception, message.str() )
    }

    CIE_END_EXCEPTION_TRACING
}


GLFWShader::~GLFWShader()
{
    glDeleteShader( this->getID() );
}


} // namespace cie::gl