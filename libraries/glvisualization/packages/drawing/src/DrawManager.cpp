#include "../inc/DrawManager.hpp"

namespace cie {
namespace gl {


DrawManager::DrawManager( GLContext& context ) :
    AbsContextClass( context, "DrawManager" ),
    _buffers( GL_STATIC_DRAW ),
    _shaders()
{
}


void DrawManager::initialize()
{
    std::vector<float> vertices = {
        -1.0f, -1.0f, 0.0f,     1.0f,0.0f,0.0f,
        1.0f, -1.0f, 0.0f,      0.0f,1.0f,0.0f,
        1.0f, 1.0f, 0.0f,       0.0f,0.0f,1.0f,
        -1.0f, 1.0f, 0.0f,      1.0f,1.0f,1.0f
    };

    std::vector<GLuint> triangles = {
        0,1,2,
        0,2,3
    };

    _buffers.writeToActiveBuffer(GL_ARRAY_BUFFER, vertices);
    _buffers.writeToActiveBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles);
}


void DrawManager::draw()
{
    glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

    GLuint error = glGetError();
    if (error!=0)
        log( std::to_string(error), CONTEXT_LOG_TYPE_ERROR );
}


void DrawManager::makeProgram()
{

    // Load vertex shader source
    GLuint vertexID     = glCreateShader( GL_VERTEX_SHADER );
    ShaderPtr shaderPtr = _shaderCode.getVertexShader();
    glShaderSource( vertexID, 1, &shaderPtr, NULL );
    _shaders.push_back( vertexID );
    log( "Load vertex shader (id=" + std::to_string(vertexID) + ")" );

    // Load geometry shader source
    GLuint geometryID   = glCreateShader( GL_GEOMETRY_SHADER );
    shaderPtr           = _shaderCode.getGeometryShader();
    glShaderSource( geometryID, 1, &shaderPtr, NULL );
    _shaders.push_back( geometryID );
    log( "Load geometry shader (id=" + std::to_string(geometryID) + ")" );
    

    // Load fragment shader source
    GLuint fragmentID   = glCreateShader( GL_FRAGMENT_SHADER );
    shaderPtr           = _shaderCode.getFragmentShader();
    glShaderSource( fragmentID, 1, &shaderPtr, NULL );
    _shaders.push_back( fragmentID );
    log( "Load fragment shader (id=" + std::to_string(fragmentID) + ")" );

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
            log("Shader compilation failed (id=" + std::to_string(id) + ")!\n" + std::string(buffer),
                CONTEXT_LOG_TYPE_ERROR );
        }
        else
            log("Shader compilation successful! (id=" + std::to_string(id) + ")" );
    }

    // Combine shaders into a program
    GLuint programID = glCreateProgram();
    glAttachShader( programID, vertexID );
    glAttachShader( programID, geometryID );
    glAttachShader( programID, fragmentID );

    glBindFragDataLocation( programID, 0, "color" );

    // Link program
    glLinkProgram( programID );
    GLint linkStatus;
    glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        char buffer[512];
        glGetProgramInfoLog( programID, 512, NULL, buffer );
        log("Program linking failed!\n" + std::string(buffer),
            CONTEXT_LOG_TYPE_ERROR );
    }
    
    glUseProgram( programID );

    // VAO
    GLuint vertexArrayObject;
    glGenVertexArrays( 1, &vertexArrayObject );
    glBindVertexArray( vertexArrayObject );

    // Buffers
    GLuint vertexBufferID   = _buffers.createBuffer();
    GLuint elementBufferID  = _buffers.createBuffer();
    _buffers.bindVertexBuffer( vertexBufferID );
    _buffers.bindElementBuffer( elementBufferID );

    // Enable vertex shader attributes
    for (size_t i=0; i<_shaderCode.attributes().size(); ++i)
    {
        GLint attributeID = glGetAttribLocation(    programID, 
                                                    _shaderCode.attributes()[i].c_str() );

        glVertexAttribPointer(  attributeID,
                                _shaderCode.attributeSizes()[i],
                                GL_FLOAT,
                                GL_FALSE,
                                _shaderCode.attributeStrides()[i] * sizeof(float),
                                (void*)(_shaderCode.attributeOffsets()[i]*sizeof(float)) );

        glEnableVertexAttribArray( attributeID );
    }
    

    // Check error
    if (glGetError()!=0)
        log( "Failed to make program", CONTEXT_LOG_TYPE_ERROR );
}


BufferHandler& DrawManager::buffers()
{
    return _buffers;
}


ShaderAssembler& DrawManager::shaderCode()
{
    return _shaderCode;
}


const std::vector<GLuint>& DrawManager::shaders() const
{
    return _shaders;
}


}
}