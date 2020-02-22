#include "../inc/DrawManager.hpp"

namespace cie {
namespace gl {


DrawManager::DrawManager( GLContext& context ) :
    ProgramManager( context, "DrawManager" ),
    _shaderManager( context ),
    _camera( context )
{
}


void DrawManager::initialize()
{
    log( "Run default draw function initialization" );
    float cLen = 0.9f;

    // Initialize uniforms
    for (auto uniform : _shaderManager.uniforms())
    {
        GLint id = glGetUniformLocation( _programID, &uniform[0] );
        _uniformIDs.push_back( id );

        GLsizei     length;
        GLsizei     size;
        GLenum      type;
        GLchar      name;
        glGetActiveUniform( _programID,
                            id,
                            0,
                            &length,
                            &size,
                            &type,
                            &name);
        
        glUniformMatrix4fv( id, 1, GL_FALSE, glm::value_ptr(_camera.transformationMatrix()) );

        //for (size_t i=0; i<4; ++i)
        //{
        //    for (size_t j=0; j<4; ++j)
        //        std::cout << std::to_string( _camera.transformationMatrix()[i][j] ) + ", ";
        //    std::cout << "\n";
        //}

        GLuint error = glGetError();
        if( error!=0 )
            logID(  "Initializing uniform \"" + uniform + "\" failed!",
                    error,
                    CONTEXT_LOG_TYPE_ERROR );
    }

    std::vector<float> vertices = {
        0.0f, 0.0, 0.0f,        1.0f, 0.0f, 0.0f,
        cLen, 0.0f, 0.0f,       0.0f, 1.0f, 0.0f,
        cLen, cLen, 0.0f,       0.0f, 0.0f, 1.0f,
        0.0f, cLen, 0.0f,       1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, cLen,       1.0f, 0.0f, 1.0f,
        cLen, 0.0f, cLen,       0.0f, 1.0f, 1.0f,
        0.0f, cLen, cLen,       1.0f, 1.0f, 1.0f

    };

    std::vector<GLuint> triangles = {
        0,1,2,
        0,2,3,
        0,3,6,
        0,6,4,
        0,4,5,
        0,5,1
    };

    _buffers.writeToActiveBuffer(GL_ARRAY_BUFFER, vertices);
    _buffers.writeToActiveBuffer(GL_ELEMENT_ARRAY_BUFFER, triangles);

    GLuint error = glGetError();
    if( error!=0 )
        logID( "Default draw function initialization failed!",
                error,
                CONTEXT_LOG_TYPE_ERROR );
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
    log( "Create callable draw function factory" );
    return [&context, this]()->void
        {
            this->draw();
        };
}



void DrawManager::compileShaders()
{
    // Compile and store shaders in the following order:
    // 1) Vertex shader
    // 2) Geometry shader
    // 3) Fragment shader

    // Load vertex shader source
    GLuint vertexID     = glCreateShader( GL_VERTEX_SHADER );
    ShaderPtr shaderPtr = _shaderManager.getVertexShader();
    glShaderSource( vertexID, 1, &shaderPtr, NULL );
    _shaders.push_back( vertexID );
    logID( "Load vertex shader", vertexID );

    // Load geometry shader source
    GLuint geometryID   = glCreateShader( GL_GEOMETRY_SHADER );
    shaderPtr           = _shaderManager.getGeometryShader();
    glShaderSource( geometryID, 1, &shaderPtr, NULL );
    _shaders.push_back( geometryID );
    logID( "Load geometry shader", geometryID );
    
    // Load fragment shader source
    GLuint fragmentID   = glCreateShader( GL_FRAGMENT_SHADER );
    shaderPtr           = _shaderManager.getFragmentShader();
    glShaderSource( fragmentID, 1, &shaderPtr, NULL );
    _shaders.push_back( fragmentID );
    logID( "Load fragment shader", fragmentID );

    // Compile shaders
    ProgramManager::compileShaders();
}



void DrawManager::makeProgram()
{
    // GL enables
    glEnable( GL_DEPTH_TEST );

    // Finalize shaders
    compileShaders();

    // Create program, attach shaders, activate program, and create VAO
    ProgramManager::makeProgram();

    // Generate and bind buffers
    GLuint vertexBufferID   = _buffers.createBuffer();
    GLuint elementBufferID  = _buffers.createBuffer();
    _buffers.bindVertexBuffer( vertexBufferID );
    _buffers.bindElementBuffer( elementBufferID );

    // Enable shader attributes
    for (size_t i=0; i<_shaderManager.attributes().size(); ++i)
    {
        GLint attributeID = glGetAttribLocation(    _programID, 
                                                    _shaderManager.attributes()[i].c_str() );

        glVertexAttribPointer(  attributeID,
                                _shaderManager.attributeSizes()[i],
                                GL_FLOAT,
                                GL_FALSE,
                                _shaderManager.attributeStrides()[i] * sizeof(float),
                                (void*)(_shaderManager.attributeOffsets()[i]*sizeof(float)) );

        glEnableVertexAttribArray( attributeID );
    }
    
    // Bind fragment shader output
    glBindFragDataLocation( _programID, 0, _shaderManager.fragOutputNames()[0].c_str() );

    // Check error
    if (glGetError()!=0)
        logID( "Failed to make program", _programID, CONTEXT_LOG_TYPE_ERROR );
}


ShaderManager& DrawManager::shaderManager()
{
    return _shaderManager;
}


const ShaderManager& DrawManager::shaderManager() const
{
    return _shaderManager;
}


Camera& DrawManager::camera()
{
    return _camera;
}


const Camera& DrawManager::camera() const
{
    return _camera;
}


}
}