#include "../inc/SpaceTreeDrawManager.hpp"


namespace cie {
namespace csg {


SpaceTreeDrawManager::SpaceTreeDrawManager( SpaceTreeNode<3,M>& root,
                                            gl::GLContext& context  ) :
    gl::DrawManager(context, "SpaceTreeDrawManager"),
    _root( &root ),
    _drawFunction( nullptr )
{
    _shaderManager.setVertexShader( "spacetreeVertexShader.glsl" );
    _shaderManager.setGeometryShader( "spacetreeGeometryShader.glsl" );
    _shaderManager.setFragmentShader( "spacetreeFragmentShader.glsl" );
    makeProgram();
}


void SpaceTreeDrawManager::initialize()
{
    DrawManager::initialize();
    log( "Run SpaceTreeDrawManager initialization" );

    std::vector<GLfloat> vertexData;

    // Define recursive node collecting function
    std::function<void(const SpaceTreeNode<3,M>&, std::vector<GLfloat>&)> collectNodes = [&collectNodes]( const SpaceTreeNode<3,M>& root, std::vector<GLfloat>& nodes )
    {
        if (root.children()[0] != nullptr )
        {
            for( const auto& child : root.children() )
                if (child != nullptr)
                    collectNodes( *child, nodes );
        }
        else
        {
            bool boundary   = false;
            bool indicator  = root.data()[0] > 0.0;
            for (const auto& data : root.data())
                if ((data>0.0) != indicator)
                {
                    boundary = true;
                    break;
                }

            if (boundary)
            {
                nodes.insert( nodes.end(), root.center().begin(), root.center().end() );
                nodes.push_back( (GLfloat)root.edgeLength() );
            }
        }
    };

    // Collect nodes
    log( "Collecting nodes..." );
    auto timerID = tic();
    collectNodes( *_root, vertexData );
    toc( "Collecting nodes took", timerID );

    log( "Write initial data to buffer" );
    _buffers.writeToActiveBuffer( GL_ARRAY_BUFFER, vertexData );
    checkGLErrors();
}


void SpaceTreeDrawManager::draw()
{
    DrawManager::draw();

    if (_drawFunction != nullptr)
        _drawFunction();
    else
        log( "Call to unset draw function!", LOG_TYPE_ERROR );

    // Draw
    GLint64 numberOfVertices;
    glGetBufferParameteri64v( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfVertices );
    numberOfVertices /= 4*sizeof(GLfloat);
    glDrawArrays( GL_POINTS, 0, numberOfVertices );
    checkGLErrors();
}


void SpaceTreeDrawManager::setDrawFunction( const std::function<void()>& function )
{
    _drawFunction = function;
}


}
}