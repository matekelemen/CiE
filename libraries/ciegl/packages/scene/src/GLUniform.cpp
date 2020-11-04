// --- External Includes ---
#include <glm/gtc/type_ptr.hpp>

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/GLUniform.hpp"
#include "ciegl/packages/utility/inc/GLError.hpp"

// --- STL Includes ---
#include <limits>


namespace cie::gl {



/* --- GLUniform --- */

GLUniform::GLUniform( const Shader::Uniform& r_shaderUniform,
                      Size programID ) :
    Shader::Uniform( r_shaderUniform ),
    utils::IDObject<GLint>( std::numeric_limits<GLint>().max() )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Find uniform in the shader program
    this->setID( glGetUniformLocation( programID, &this->_name[0] ) );

    // Query uniform properties
    GLchar tmp;
    glGetActiveUniform( programID,
                        this->getID(),
                        0,
                        &this->_properties.length,
                        &this->_properties.size,
                        &this->_properties.type,
                        &tmp);

    checkGLErrors( "Failed to initialize uniform: " + this->_name );

    CIE_END_EXCEPTION_TRACING
}


const GLUniform::Properties& GLUniform::properties() const
{
    return this->_properties;
}




/* --- GLUniformPlaceholder --- */

GLUniformPlaceholder::GLUniformPlaceholder( const Shader::Uniform& r_shaderUniform,
                                            Size programID ) :
    GLUniform( r_shaderUniform, programID )
{
}


void GLUniformPlaceholder::write()
{
    CIE_THROW( Exception, "Attempt to write to unbound uniform: " + this->_name )
}




/* --- FloatMat4GLUniform --- */

FloatMat4GLUniform::FloatMat4GLUniform( const Shader::Uniform& r_shaderUniform,
                                        Size programID,
                                        const FloatMat4GLUniform::uniform_type& r_uniform ) :
    GLUniform( r_shaderUniform, programID ),
    _r_uniform( r_uniform )
{
    CIE_CHECK(
        this->_properties.type == GL_FLOAT_MAT4,
        "Uniform type mismatch"
    )
}


void FloatMat4GLUniform::write()
{
    CIE_BEGIN_EXCEPTION_TRACING

    glUniformMatrix4fv( this->getID(), 
                        1,
                        GL_FALSE, 
                        glm::value_ptr(this->_r_uniform) );

    checkGLErrors( "Failed to write uniform" );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl