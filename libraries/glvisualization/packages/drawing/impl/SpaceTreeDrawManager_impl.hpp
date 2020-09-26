// --- Internal Includes ---
#include "glvisualization/packages/drawing/inc/SpaceTreeDrawManager.hpp"


namespace cie::gl {


template <size_t M>
SpaceTreeDrawManager<M>::SpaceTreeDrawManager(  csg::SpaceTreeNode<3,M>& root,
                                                GLContext& context  ) :
    DrawManager(context, "SpaceTreeDrawManager"),
    _root( &root ),
    _drawFunction( nullptr )
{
    _shaderManager.setVertexShader( "pointVertexShader.glsl" );
    _shaderManager.setGeometryShader( "pointGeometryShader.glsl" );
    _shaderManager.setFragmentShader( "pointFragmentShader.glsl" );
    makeProgram();
}


template <size_t M>
void SpaceTreeDrawManager<M>::collectNodesToBuffer()
{
    std::vector<GLfloat> vertexData;

    // Define recursive node collecting function
    std::function<void(const csg::SpaceTreeNode<3,M>&, std::vector<GLfloat>&)> collectNodes = [&collectNodes]( const csg::SpaceTreeNode<3,M>& root, std::vector<GLfloat>& nodes )
    {
        if (root.children()[0] != nullptr )
        {
            #pragma omp task shared(root,nodes)
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
                #pragma omp critical(SpaceTreeDrawManager_collectNodes)
                nodes.insert( nodes.end(), root.center().begin(), root.center().end() );
                // Uncomment for complete cell representation (needs matching spacetree shaders)
                //nodes.push_back( (GLfloat)root.edgeLength() );
            }
        }
    };

    // Collect nodes and write to buffer
    #pragma omp parallel
    #pragma omp single
    collectNodes( *_root, vertexData );

    _buffers.writeToActiveBuffer( GL_ARRAY_BUFFER, vertexData );
    checkGLErrors();
}


template <size_t M>
void SpaceTreeDrawManager<M>::initialize()
{
    DrawManager::initialize();
    log( "Run SpaceTreeDrawManager initialization" );
    collectNodesToBuffer();
}


template <size_t M>
bool SpaceTreeDrawManager<M>::draw()
{
    DrawManager::draw();

    bool result = false;
    if (_drawFunction != nullptr)
        result = _drawFunction();
    else
        log( "Call to unset draw function!", LOG_TYPE_ERROR );

    // Draw
    GLint64 numberOfVertices;
    glGetBufferParameteri64v( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &numberOfVertices );
    //numberOfVertices /= 4*sizeof(GLfloat);
    numberOfVertices /= 3*sizeof(GLfloat);
    glDrawArrays( GL_POINTS, 0, numberOfVertices );

    checkGLErrors();
    return result;
}


template <size_t M>
void SpaceTreeDrawManager<M>::setDrawFunction( const std::function<bool()>& function )
{
    _drawFunction = function;
}


} // namespace cie::gl