// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/GLFWScene.hpp"

// --- STL Includes ---
#include <limits>


namespace cie::gl {


GLFWScene::GLFWScene( utils::Logger& r_logger,
                      ShaderPtr p_vertexShader,
                      ShaderPtr p_geometryShader,
                      ShaderPtr p_fragmentShader ) :
    Scene( r_logger, "GLFWScene" ),
    utils::IDObject<Size>( std::numeric_limits<Size>().max() ),
    _vaoID( std::numeric_limits<GLuint>().max() )
{
    CIE_BEGIN_EXCEPTION_TRACING

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
    if ( !this->_p_bufferManager )
        this->logID( "Unset buffer manager!",
                     this->getID(),
                     LOG_TYPE_ERROR );

    if ( !this->_p_bufferManager->hasBoundVertexBuffer() )
        this->logID( "Unset vertex buffer!",
                     this->getID(),
                     LOG_TYPE_ERROR );

    if ( !this->_p_bufferManager->hasBoundElementBuffer() )
        this->logID( "Unset element buffer",
                     this->getID(),
                     LOG_TYPE_ERROR );

    // Handle shader attributes
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

    for ( const auto& r_output : this->_p_fragmentShader->outputs() )
        glBindFragDataLocation( this->getID(),
                                r_output.getID(),
                                r_output.name().c_str() );

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

    CIE_END_EXCEPTION_TRACING
}


GLFWScene::~GLFWScene()
{
    this->logID( "Delete program", this->getID() );
    glDeleteProgram( this->getID() );

    this->logID( "Delete Vertex Array Object", this->_vaoID );
    glDeleteVertexArrays( 1, &_vaoID );
}


} // namespace cie::gl