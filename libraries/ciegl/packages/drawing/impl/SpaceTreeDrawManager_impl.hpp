// --- Internal Includes ---
#include "ciegl/packages/drawing/inc/SpaceTreeDrawManager.hpp"


namespace cie::gl {


template <class NodeType>
SpaceTreeDrawManager<NodeType>::SpaceTreeDrawManager(   NodeType& r_root,
                                                        GLContext& r_context  ) :
    DrawManager( r_context, "SpaceTreeDrawManager" ),
    _r_root( r_root ),
    _drawFunction( nullptr )
{
    _shaderManager.setVertexShader( "pointVertexShader.glsl" );
    _shaderManager.setGeometryShader( "pointGeometryShader.glsl" );
    _shaderManager.setFragmentShader( "pointFragmentShader.glsl" );
    makeProgram();
}


template <class NodeType>
void SpaceTreeDrawManager<NodeType>::collectNodesToBuffer()
{
    std::vector<GLfloat> vertexData;

    // Define recursive node collecting function
    auto collectNodes = [&vertexData]( NodeType* p_root ) -> Bool
    {
        if ( p_root->children().empty() && p_root->isBoundary() )
        {

            typename NodeType::point_type center = getCenter( *p_root );

            vertexData.insert(  vertexData.end(),
                                center.begin(),
                                center.end() );
            // Uncomment for complete cell representation (needs matching spacetree shaders)
            //nodes.push_back( (GLfloat)root.edgeLength() );
        }
        return true;
    };

    // Collect nodes and write to buffer
    #pragma omp parallel
    #pragma omp single
    _r_root.visit( collectNodes );

    _buffers.writeToActiveBuffer( GL_ARRAY_BUFFER, vertexData );
    checkGLErrors();
}



template <class NodeType>
void SpaceTreeDrawManager<NodeType>::initialize()
{
    DrawManager::initialize();
    log( "Run SpaceTreeDrawManager initialization" );
    collectNodesToBuffer();
}



template <class NodeType>
bool SpaceTreeDrawManager<NodeType>::draw()
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



template <class NodeType>
void SpaceTreeDrawManager<NodeType>::setDrawFunction( const std::function<bool()>& function )
{
    _drawFunction = function;
}


template <concepts::Cube PrimitiveType>
typename PrimitiveType::point_type getCenter( const PrimitiveType& r_primitive )
{
    auto center     = r_primitive.base();
    auto halfLength = r_primitive.length() / 2.0;
    for ( auto& r_component : center )
        r_component += halfLength;

    return center;
}



template <concepts::Box PrimitiveType>
typename PrimitiveType::point_type getCenter( const PrimitiveType& r_primitive )
{
    auto center     = r_primitive.base();
    auto it_length  = r_primitive.lengths().begin();

    for ( auto& r_component : center )
        r_component += (*it_length++) / 2.0;

    return center;
}


} // namespace cie::gl