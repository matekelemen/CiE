// --- Internal Includes ---
#include "ciegl/packages/drawing/inc/DrawManager.hpp"

namespace cie::gl {


DrawManager::DrawManager( GLContext& context, const std::string& className ) :
    ProgramManager( context, className ),
    _shaderManager( context ),
    _camera( new Camera(context) ),
    _transformationID(-1)
{
    _buffers.setDrawMode( GL_DYNAMIC_DRAW );
}


DrawManager::~DrawManager()
{
}


void DrawManager::initialize()
{
    log( "Run default draw function initialization" );

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
        
        if (type == GL_FLOAT_MAT4)
        {
            _transformationID = id;
            glUniformMatrix4fv( id, 
                                1, 
                                GL_FALSE, 
                                glm::value_ptr(_camera->transformationMatrix()) );
        }

        checkGLErrors( "Initializing uniform \"" + uniform + "\" failed!" );
    }
}


bool DrawManager::draw()
{
    // Set background and clear
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Set transformation
    if (_transformationID >= 0)
        glUniformMatrix4fv( _transformationID, 
                            1, 
                            GL_FALSE, 
                            glm::value_ptr(_camera->transformationMatrix()) );
    else
        logID( "Unset transformation matrix ID!", _transformationID, LOG_TYPE_ERROR );

    return true;
}


DrawFunction DrawManager::makeDrawFunction( GLContext& context )
{
    log( "Create callable draw function factory" );
    return [&context, this]() -> bool
        {
            return this->draw();
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
        logID( "Failed to make program", _programID, LOG_TYPE_ERROR );

    // Validate program
    glValidateProgram( _programID );
    GLint programStatus;
    glGetProgramiv( _programID, GL_VALIDATE_STATUS, &programStatus );
    if (programStatus == GL_TRUE)
        logID( "Program validated successfully", _programID );
    else
    {
        GLsizei messageLength;
        GLchar buffer[512];
        glGetProgramInfoLog(    _programID,
                                sizeof(buffer)/sizeof(GLchar),
                                &messageLength,
                                buffer );
        log(    "Program validation failed! | ID_" + std::to_string(_programID) + "\n"
                    + std::string(buffer),
                LOG_TYPE_ERROR );
    }
}


ShaderManager& DrawManager::shaderManager()
{
    return _shaderManager;
}


const ShaderManager& DrawManager::shaderManager() const
{
    return _shaderManager;
}


CameraPtr& DrawManager::camera()
{
    return _camera;
}


const CameraPtr& DrawManager::camera() const
{
    return _camera;
}


} // namespace cie::gl