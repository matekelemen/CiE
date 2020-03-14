#include "../inc/ProgramManager.hpp"

namespace cie {
namespace gl {


ProgramManager::ProgramManager( GLContext& context, const std::string& className ) :
    AbsContextClass( context, className ),
    _buffers( context, GL_STATIC_DRAW ),
    _shaders( ),
    _programID(0),
    _vaoID(0)
{
}


ProgramManager::~ProgramManager()
{
    logID( "Delete program", _programID );
    glDeleteProgram( _programID );

    for (auto shaderID : _shaders)
    {
        logID( "Delete shader", shaderID );
        glDeleteShader(shaderID);
    }

    logID( "Delete VAO", _vaoID );
    glDeleteVertexArrays( 1, &_vaoID );
}


void ProgramManager::initialize()
{
    log( "Attempt to initialize ProgramManager" );
}


void ProgramManager::compileShaders( )
{
    // Compile shaders
    for ( GLuint id : _shaders )
    {
        glCompileShader( id );
        GLint compileStatus;
        glGetShaderiv( id, GL_COMPILE_STATUS, &compileStatus );
        if (compileStatus != GL_TRUE)
        {
            char buffer[512];
            glGetShaderInfoLog( id, 512, NULL, buffer );
            log(    "Shader compilation failed | ID_" + std::to_string(id) + "\n" + 
                    std::string(buffer), 
                    LOG_TYPE_ERROR );
        }
        else
            logID("Shader compilation successful!", id );
    }
}


void ProgramManager::makeProgram( )
{
    // Combine shaders into a program
    _programID = glCreateProgram();
    for (auto shaderID : _shaders)
        glAttachShader( _programID, shaderID );
    logID( "Create program", _programID );

    // Link program
    glLinkProgram( _programID );
    GLint linkStatus;
    glGetProgramiv(_programID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        char buffer[512];
        glGetProgramInfoLog( _programID, 512, NULL, buffer );
        log("Program linking failed!\n" + std::string(buffer),
            LOG_TYPE_ERROR );
    }
    else
        logID( "Program linked successfully", _programID );

    // Activate program
    glUseProgram( _programID );
    logID( "Set active program", _programID );

    // VAO
    glGenVertexArrays( 1, &_vaoID );
    glBindVertexArray( _vaoID );
    logID( "Create VAO", _vaoID );
}


BufferHandler& ProgramManager::buffers()
{
    return _buffers;
}


const BufferHandler& ProgramManager::buffers() const
{
    return _buffers;
}


const std::vector<GLuint>& ProgramManager::shaders() const
{
    return _shaders;
}



}
}