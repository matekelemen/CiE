// --- Internal Includes ---
#include "ciegl/packages/buffer/inc/GLFWBufferManager.hpp"

namespace cie::gl {


GLFWBufferManager::GLFWBufferManager( utils::Logger& r_logger,
                                      GLuint drawMode ) :
    AbsBufferManager( r_logger, "GLFWBufferManager" ),
    _buffers( )
{
    this->setDrawMode( drawMode );
}


GLFWBufferManager::GLFWBufferManager( utils::Logger& r_logger ) :
    GLFWBufferManager( r_logger, GL_DYNAMIC_DRAW )
{
}


GLFWBufferManager::~GLFWBufferManager()
{
    for (auto bufferID : _buffers)
    {
        logID( "Delete buffer", bufferID );
        glDeleteBuffers(1, &bufferID);
    }
    _buffers = {};
}


GLuint GLFWBufferManager::createBuffer( )
{
    GLuint bufferID;
    glGenBuffers( 1, &bufferID );
    _buffers.push_back( bufferID );
    logID( "Create buffer", bufferID );

    return bufferID;
}


void GLFWBufferManager::bindVertexBuffer( GLuint bufferID )
{
    glBindBuffer( GL_ARRAY_BUFFER, bufferID );
    logID( "Bind vertex buffer", bufferID );
}


void GLFWBufferManager::bindElementBuffer( GLuint bufferID )
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferID );
    logID( "Bind element buffer", bufferID );
}


void GLFWBufferManager::setDrawMode( GLuint drawMode )
{
    _drawMode = drawMode;
    switch (drawMode)
    {
        case GL_STATIC_DRAW:
            log( "Set GL_STATIC_DRAW");
            break;
        case GL_DYNAMIC_DRAW:
            log( "Set GL_DYNAMIC_DRAW");
            break;
        case GL_STREAM_DRAW:
            log( "Set GL_STREAM_DRAW");
            break;
        default:
            log( "Attempt to set invalid draw mode: " + std::to_string(drawMode), LOG_TYPE_ERROR );
            break;
    }
    
}


} // namespace cie::gl