// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Scene.hpp"
#include "ciegl/packages/buffer/inc/GLFWBuffer.hpp"
#include "ciegl/packages/utility/inc/GLError.hpp"

// --- STL Includes ---
#include <algorithm>
#include <limits>


namespace cie::gl {


Scene::Scene( utils::Logger& r_logger,
              const std::string& r_name,
              ShaderPtr p_vertexShader,
              ShaderPtr p_geometryShader,
              ShaderPtr p_fragmentShader,
              BufferManagerPtr p_bufferManager,
              VertexBufferPtr p_vertexBuffer,
              ElementBufferPtr p_elementBuffer ) :
    utils::Loggee( r_logger, r_name ),
    utils::IDObject<Size>( std::numeric_limits<Size>().max() ),
    _cameras(),
    _p_vertexShader( p_vertexShader ),
    _p_geometryShader( p_geometryShader ),
    _p_fragmentShader( p_fragmentShader ),
    _p_bufferManager( p_bufferManager ),
    _uniforms(),
    _vaoID( std::numeric_limits<GLuint>().max() )
{
    CIE_BEGIN_EXCEPTION_TRACING

    glEnable( GL_DEPTH_TEST );

    // Set shaders
    this->setVertexShader( p_vertexShader );
    this->setGeometryShader( p_geometryShader );
    this->setFragmentShader( p_fragmentShader );

    // New program
    this->setID( glCreateProgram() );
    this->logID( "Build new program", this->getID() );

    // Shaders
    if ( this->_p_vertexShader )
        glAttachShader( this->getID(), _p_vertexShader->getID() );
    else
        this->logID( "Unset vertex shader!",
                        this->getID(),
                        LOG_TYPE_ERROR );

    if ( this->_p_geometryShader )
        glAttachShader( this->getID(), _p_geometryShader->getID() );
    
    if ( this->_p_fragmentShader )
        glAttachShader( this->getID(), _p_fragmentShader->getID() );
    else
        this->logID( "Unset fragment shader!",
                        this->getID(),
                        LOG_TYPE_ERROR );

    // Link and check for errors
    glLinkProgram( this->getID() );

    GLint linkStatus;
    glGetProgramiv( this->getID(), GL_LINK_STATUS, &linkStatus );

    if ( linkStatus != GL_TRUE )
    {
        char buffer[512];
        glGetProgramInfoLog( this->getID(),
                                512,
                                NULL,
                                buffer );
        this->logID( "Program linking failed!\n" + std::string(buffer),
                        this->getID(),
                        LOG_TYPE_ERROR );
    }

    // Set as active program
    glUseProgram( this->getID() );
    this->logID( "Set active program", this->getID() );

    // Vertex Array Object
    // TODO: wtf? also, is it okay here?
    glGenVertexArrays( 1, &this->_vaoID );
    this->logID( "Create Vertex Array Object", this->_vaoID );

    glBindVertexArray( this->_vaoID );
    this->logID( "Bind Vertex Array Object", this->_vaoID );

    // Check buffers
    //  - no buffers should be bound before binding the vertex array object
    this->setBufferManager( p_bufferManager );

    if ( !this->_p_bufferManager )
        this->logID( "Unset buffer manager!",
                     this->getID(),
                     LOG_TYPE_ERROR );

    if ( this->_p_bufferManager->hasBoundVertexBuffer() )
        this->logID( "Bound vertex buffer before a vertex array object was created!",
                     this->getID(),
                     LOG_TYPE_WARNING );

    if ( this->_p_bufferManager->hasBoundElementBuffer() )
        this->logID( "Bound element buffer before a vertex array object was created!",
                     this->getID(),
                     LOG_TYPE_WARNING );

    // Initialize and bind buffers
    if ( !p_vertexBuffer )
        p_vertexBuffer = this->_p_bufferManager->makeVertexBuffer();
    
    if ( !p_elementBuffer )
        p_elementBuffer = this->_p_bufferManager->makeElementBuffer();

    this->_p_bufferManager->bindVertexBuffer( p_vertexBuffer );
    this->_p_bufferManager->bindElementBuffer( p_elementBuffer );

    // Map vertex shader attributes
    for ( const auto& r_attribute : this->_p_vertexShader->attributes() )
    {
        GLint attributeID = glGetAttribLocation( this->getID(),
                                                 r_attribute.name().c_str() );

        glVertexAttribPointer( attributeID,
                               r_attribute.size(),
                               GL_FLOAT,
                               GL_FALSE,
                               r_attribute.stride() * sizeof(float),
                               (void*)(r_attribute.offset() * sizeof(float)) );

        glEnableVertexAttribArray( attributeID );
    }


    // Bind fragment shader outputs
    for ( const auto& r_output : this->_p_fragmentShader->outputs() )
        glBindFragDataLocation( this->getID(),
                                r_output.getID(),
                                r_output.name().c_str() );

    // Create placeholders for uniforms
    for ( const auto& r_uniform : this->_p_vertexShader->uniforms() )
        this->_uniforms.emplace_back(
            new GLUniformPlaceholder( r_uniform, this->getID() )
        );

    // Check gl errors
    if ( glGetError() != 0 )
        this->logID( "Failed to make GL program",
                     this->getID(),
                     LOG_TYPE_ERROR );

    // Validate program
    glValidateProgram( this->getID() );

    GLint programStatus;
    glGetProgramiv( this->getID(), GL_VALIDATE_STATUS, &programStatus );

    if ( programStatus == GL_TRUE )
        this->logID( "Program successfully validated", this->getID() );
    else
    {
        GLsizei messageLength;
        GLchar buffer[512];
        glGetProgramInfoLog( this->getID(),
                             sizeof( buffer ) / sizeof( GLchar ),
                             &messageLength,
                             buffer );
        this->log( "Program validation failed | ID_" + std::to_string(this->getID()),
                   LOG_TYPE_ERROR );
    }

    // Initial clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    CIE_END_EXCEPTION_TRACING
}


Scene::~Scene()
{
    this->logID( "Delete program", this->getID() );
    glDeleteProgram( this->getID() );

    this->logID( "Delete Vertex Array Object", this->_vaoID );
    glDeleteVertexArrays( 1, &_vaoID );
}


void Scene::update()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Update all cameras
    for ( auto& rp_camera : this->_cameras )
        rp_camera->update();

    // Check whether the number of vertex buffers
    // and element buffers is identical
    Size numberOfBuffers = this->_p_bufferManager->elementBuffers().size();

    if ( this->_p_bufferManager->vertexBuffers().size() != numberOfBuffers ) [[unlikely]]
        CIE_THROW( OutOfRangeException, "Buffer size mismatch" )

    // Save current bound buffers for resetting after the update
    VertexBufferPtr p_boundVertexBuffer = this->_p_bufferManager->boundVertexBuffer();
    ElementBufferPtr p_boundElementBuffer = this->_p_bufferManager->boundElementBuffer();

    // Update uniforms
    for ( const auto& rp_uniform : this->_uniforms )
        rp_uniform->write();

    checkGLErrors( *this, "Error updating uniforms" );

    // Loop through buffers and draw their contents
    auto it_vertexBuffer    = this->_p_bufferManager->vertexBuffers().begin();
    auto it_elementBuffer   = this->_p_bufferManager->elementBuffers().begin();
    auto it_vertexBufferEnd = this->_p_bufferManager->vertexBuffers().end();

    for ( ; it_vertexBuffer!=it_vertexBufferEnd; ++it_vertexBuffer, ++it_elementBuffer )
    {
        this->_p_bufferManager->bindVertexBuffer( 
            *it_vertexBuffer
        );
        this->_p_bufferManager->bindElementBuffer( 
            *it_elementBuffer
        );

        GLint64 numberOfIndices;
        glGetBufferParameteri64v( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfIndices );
        numberOfIndices /= sizeof( GLuint );

        glDrawElements( GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0 );

        checkGLErrors( *this,
                       "Error drawing from buffer" );
    }

    // Reset bound buffers
    this->_p_bufferManager->bindVertexBuffer( p_boundVertexBuffer );
    this->_p_bufferManager->bindElementBuffer( p_boundElementBuffer );

    CIE_END_EXCEPTION_TRACING
}


void Scene::addCamera( CameraPtr p_camera )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it_camera = std::find(
        _cameras.begin(),
        _cameras.end(),
        p_camera
    );

    if ( it_camera != _cameras.end() )
        CIE_THROW( Exception, "Camera is already part of this Scene!" )

    _cameras.push_back( p_camera );

    CIE_END_EXCEPTION_TRACING
}


void Scene::removeCamera( CameraPtr p_camera )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it_camera = std::find(
        _cameras.begin(),
        _cameras.end(),
        p_camera
    );

    if ( it_camera == _cameras.end() )
        CIE_THROW( OutOfRangeException, "Attempt to remove non-existent camera" )

    _cameras.erase( it_camera );

    CIE_END_EXCEPTION_TRACING
}


const typename Scene::camera_container& Scene::cameras() const
{
    return _cameras;
}


Scene& Scene::setVertexShader( ShaderPtr p_vertexShader )
{
    _p_vertexShader = p_vertexShader;
    return *this;
}


Scene& Scene::setGeometryShader( ShaderPtr p_geometryShader )
{
    _p_geometryShader = p_geometryShader;
    return *this;
}


Scene& Scene::setFragmentShader( ShaderPtr p_fragmentShader )
{
    _p_fragmentShader = p_fragmentShader;
    return *this;
}


const ShaderPtr& Scene::vertexShader() const
{
    return _p_vertexShader;
}


const ShaderPtr& Scene::geometryShader() const
{
    return _p_geometryShader;
}


const ShaderPtr& Scene::fragmentShader() const
{
    return _p_fragmentShader;
}


Scene& Scene::setBufferManager( BufferManagerPtr p_bufferManager )
{
    _p_bufferManager = p_bufferManager;
    return *this;
}


const BufferManagerPtr& Scene::bufferManager() const
{
    return _p_bufferManager;
}


const Scene::uniform_container& Scene::uniforms() const
{
    return this->_uniforms;
}


void Scene::bindUniform( const std::string& r_name,
                         const glm::mat4& r_uniform )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Find uniform in the internal list of uniforms
    GLUniformPtr& rp_uniform = this->findUniform( r_name );

    // Check uniform type
    CIE_CHECK(
        rp_uniform->properties().type == GL_FLOAT_MAT4,
        "Attempt to bind mismatching uniform types: " + r_name
    )

    // Replace the uniform
    rp_uniform.reset(
        new FloatMat4GLUniform( *rp_uniform,
                                this->getID(),
                                r_uniform )
    );

    CIE_END_EXCEPTION_TRACING
}


GLUniformPtr& Scene::findUniform( const std::string& r_name )
{
    auto itp_uniform = std::find_if(
        this->_uniforms.begin(),
        this->_uniforms.end(),
        [&r_name]( const GLUniformPtr& rp_uniform ) -> bool
        {
            if ( rp_uniform->name() == r_name )     [[unlikely]]
                return true;
            else                                    [[likely]]
                return false;
        }
    );

    CIE_CHECK(
        itp_uniform != this->_uniforms.end(),
        "Uniform not found: " + r_name
    )

    return *itp_uniform;
}


} // namespace cie::gl