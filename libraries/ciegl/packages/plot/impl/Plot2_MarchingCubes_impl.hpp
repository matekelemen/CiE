#ifndef CIE_GL_PLOT_2_MARCHING_CUBES_IMPL_HPP
#define CIE_GL_PLOT_2_MARCHING_CUBES_IMPL_HPP

// -- LinAlg Includes ---
#include "linalg/packages/overloads/inc/vectoroperators.hpp"

// --- STL Includes ---
#include <mutex>


namespace cie::gl {


template <concepts::MarchingPrimitives ObjectType>
requires concepts::Cube<typename ObjectType::primitive_type>
Plot2<ObjectType>::Plot2( ObjectType& r_marchingCubes ) :
    AbsPlot2<std::true_type>()
{
    CIE_BEGIN_EXCEPTION_TRACING

    using VertexType = typename Plot2<ObjectType>::vertex_type;
    using ::operator+;
    using ::operator/;

    // Init
    this->_p_scene->setDrawMode( GL_LINES );

    std::mutex mutex;
    ElementBuffer::data_type indexCounter = 0;

    auto registerLine = [this, &mutex, &r_marchingCubes, &indexCounter]( Size lineIndex, const typename ObjectType::output_arguments& r_edges )
    -> void
    {
        std::scoped_lock lock(mutex);
        
        for ( const auto& r_edge : r_edges )
        {
            auto point = (r_marchingCubes.getVertex( lineIndex, r_edge.first ) + r_marchingCubes.getVertex( lineIndex, r_edge.second ) ) / 2.0;

            this->_vertices.push_back( makeVertex<VertexType>(
                this->_p_attributes,
                point[0],
                point[1]
            ) );
            this->_p_indices->push_back( indexCounter++ );
        }
    };

    r_marchingCubes.setOutputFunctor( registerLine );
    r_marchingCubes.execute();

    this->fit( true );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::MarchingPrimitives ObjectType>
requires concepts::Cube<typename ObjectType::primitive_type>
void plot2( ObjectType& r_marchingCubes )
{
    Plot2<ObjectType>( r_marchingCubes ).show();
}


} // namespace cie::gl


#endif