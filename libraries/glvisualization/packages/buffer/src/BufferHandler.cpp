#include "../inc/BufferHandler.hpp"

namespace cie {
namespace gl {


BufferHandler::BufferHandler(   GLuint drawMode,
                                GLuint numberOfBuffers ) :
    _buffers( numberOfBuffers ),
    _drawMode( drawMode )
{
}


GLuint BufferHandler::createBuffer( )
{
    GLuint bufferID;
    glGenBuffers( 1, &bufferID );
    _buffers.push_back( bufferID );
    return bufferID;
}


void BufferHandler::bindVertexBuffer( GLuint bufferID )
{
    glBindBuffer( GL_ARRAY_BUFFER, bufferID );
}


void BufferHandler::bindElementBuffer( GLuint bufferID )
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferID );
}


void BufferHandler::setDrawMode( GLuint drawMode )
{
    _drawMode = drawMode;
}


}
}