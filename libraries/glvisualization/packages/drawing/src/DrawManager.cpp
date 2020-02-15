#include "../inc/DrawManager.hpp"

namespace cie {
namespace gl {


DrawManager::DrawManager( GLContext& context ) :
    AbsContextClass( context, "DrawManager" ),
    _buffers( context, GL_STREAM_DRAW ),
    _shaderCode( context ),
    _shaders( ),
    _programID(0),
    _vaoID(0)
{
}


DrawManager::~DrawManager()
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


void DrawManager::initialize()
{
    std::vector<float> vertices = {
        -0.9f, -0.9f, 0.0f,     1.0f,0.0f,0.0f,
        0.9f, -0.9f, 0.0f,      0.0f,1.0f,0.0f,
        0.9f, 0.9f, 0.0f,       0.0f,0.0f,1.0f,
        -0.9f, 0.9f, 0.0f,      1.0f,1.0f,1.0f
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
    // Set background and clear
    glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Get number of elements to draw
    GLint64 numberOfElements;
    glGetBufferParameteri64v( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfElements );
    numberOfElements /= sizeof(GLuint);

    // Draw buffer
    glDrawElements( GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, 0 );
}


DrawFunction DrawManager::makeDrawFunction( GLContext& context )
{
    return [&context, this]()->void
        {
            this->draw();
        };
}



void DrawManager::compileShaders()
{
    // Load vertex shader source
    GLuint vertexID     = glCreateShader( GL_VERTEX_SHADER );
    ShaderPtr shaderPtr = _shaderCode.getVertexShader();
    glShaderSource( vertexID, 1, &shaderPtr, NULL );
    _shaders.push_back( vertexID );
    logID( "Load vertex shader", vertexID );

    // Load geometry shader source
    GLuint geometryID   = glCreateShader( GL_GEOMETRY_SHADER );
    shaderPtr           = _shaderCode.getGeometryShader();
    glShaderSource( geometryID, 1, &shaderPtr, NULL );
    _shaders.push_back( geometryID );
    logID( "Load geometry shader", geometryID );
    

    // Load fragment shader source
    GLuint fragmentID   = glCreateShader( GL_FRAGMENT_SHADER );
    shaderPtr           = _shaderCode.getFragmentShader();
    glShaderSource( fragmentID, 1, &shaderPtr, NULL );
    _shaders.push_back( fragmentID );
    logID( "Load fragment shader", fragmentID );

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
                    CONTEXT_LOG_TYPE_ERROR );
        }
        else
            logID("Shader compilation successful!", id );
    }
}



void DrawManager::makeProgram()
{
    // GL enables
    glEnable( GL_DEPTH_TEST );

    // Finalize shaders
    compileShaders();

    // Combine shaders into a program
    _programID = glCreateProgram();
    for (auto shaderID : _shaders)
        glAttachShader( _programID, shaderID );
    logID( "Create program", _programID );

    // Bind fragment shader output
    glBindFragDataLocation( _programID, 0, _shaderCode.fragOutputNames()[0].c_str() );

    // Link program
    glLinkProgram( _programID );
    GLint linkStatus;
    glGetProgramiv(_programID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        char buffer[512];
        glGetProgramInfoLog( _programID, 512, NULL, buffer );
        log("Program linking failed!\n" + std::string(buffer),
            CONTEXT_LOG_TYPE_ERROR );
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

    // Generate and bind buffers
    GLuint vertexBufferID   = _buffers.createBuffer();
    GLuint elementBufferID  = _buffers.createBuffer();
    _buffers.bindVertexBuffer( vertexBufferID );
    _buffers.bindElementBuffer( elementBufferID );

    // Enable vertex shader attributes
    for (size_t i=0; i<_shaderCode.attributes().size(); ++i)
    {
        GLint attributeID = glGetAttribLocation(    _programID, 
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
        logID( "Failed to make program", _programID, CONTEXT_LOG_TYPE_ERROR );
}


BufferHandler& DrawManager::buffers()
{
    return _buffers;
}


ShaderManager& DrawManager::shaderCode()
{
    return _shaderCode;
}


const std::vector<GLuint>& DrawManager::shaders() const
{
    return _shaders;
}


}
}