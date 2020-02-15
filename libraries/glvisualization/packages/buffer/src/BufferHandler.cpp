#include "../inc/BufferHandler.hpp"

namespace cie {
namespace gl {


BufferHandler::BufferHandler(   GLContext& context,
                                GLuint drawMode,
                                GLuint numberOfBuffers ) :
    AbsContextClass( context, "BufferHandler" ),
    _buffers( numberOfBuffers ),
    _drawMode( drawMode )
{
}


GLuint BufferHandler::createBuffer( )
{
    GLuint bufferID;
    glGenBuffers( 1, &bufferID );
    _buffers.push_back( bufferID );
    logID( "Create buffer", bufferID );

    return bufferID;
}


void BufferHandler::bindVertexBuffer( GLuint bufferID )
{
    glBindBuffer( GL_ARRAY_BUFFER, bufferID );
    logID( "Bind vertex buffer", bufferID );
}


void BufferHandler::bindElementBuffer( GLuint bufferID )
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferID );
    logID( "Bind element buffer", bufferID );
}


void BufferHandler::setDrawMode( GLuint drawMode )
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
            log( "Attempt to set invalid draw mode: " + std::to_string(drawMode), CONTEXT_LOG_TYPE_ERROR );
            break;

    }
    
}


}
}