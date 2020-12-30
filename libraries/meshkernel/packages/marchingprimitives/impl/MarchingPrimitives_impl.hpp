#ifndef CIE_MESHKERNEL_MARCHING_PRIMITIVES_IMPL_HPP
#define CIE_MESHKERNEL_MARCHING_PRIMITIVES_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/maths/inc/bit.hpp"

#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- LinAlg Includes ---
#include "linalg/packages/overloads/inc/vectoroperators.hpp"


namespace cie::mesh {


template <concepts::CSGObject TargetType>
MarchingPrimitives<TargetType>::MarchingPrimitives( typename MarchingPrimitives<TargetType>::target_ptr p_target,
                                                    const typename MarchingPrimitives<TargetType>::edge_table r_edgeTable,
                                                    const typename MarchingPrimitives<TargetType>::connectivity_table& r_connectivityTable,
                                                    typename MarchingPrimitives<TargetType>::output_functor outputFunctor ) :
    _p_target( p_target ),
    _connectivityTable( r_connectivityTable ),
    _outputFunctor( outputFunctor )
{
}


template <concepts::CSGObject TargetType>
void
MarchingPrimitives<TargetType>::execute()
{
    CIE_BEGIN_EXCEPTION_TRACING

    using coordinate_type = MarchingPrimitives<TargetType>::coordinate_type;
    using point_type      = MarchingPrimitives<TargetType>::point_type;

    using cie::operator+<point_type>;
    using cie::operator/<point_type,coordinate_type>;

    typename MarchingPrimitives<TargetType>::point_container vertices;
    typename MarchingPrimitives<TargetType>::output_arguments outputArguments;

    while ( this->getNextPrimitive(vertices) )
    {
        Size configurationIndex = 0;

        // Evaluate target and build configuration
        for ( Size vertexIndex=0; vertexIndex<vertices.size(); ++vertexIndex )
            if ( this->_p_target->at( vertices[vertexIndex] ) )
                utils::flipBit( configurationIndex, vertexIndex );

        // Find surface primitive constructor map
        const auto& r_primitives = this->_connectivityTable[configurationIndex];

        // Build surface primitives
        for ( const auto& r_primitive : r_primitives )
        {
            for ( auto& r_point : outputArguments )
            {
                const auto& r_edge = r_primitive[dim];
                r_point = (vertices[r_edge.first] + vertices[r_edge.second]) / coordinate_type(2);
            }

            this->_outputFunctor( outputArguments );
        }
    }

    CIE_END_EXCEPTION_TRACING
}


template <concepts::CSGObject TargetType>
void
MarchingPrimitives<TargetType>::checkIfInitialized() const
{
    CIE_CHECK_POINTER( this->_p_target )
    CIE_CHECK( !this->_r_connectivityTable.empty(), "Empty connectivity table!" )
    CIE_CHECK( this->_outputFunctor != nullptr, "Unset output functor!" )
}


} // namespace cie::mesh


#endif