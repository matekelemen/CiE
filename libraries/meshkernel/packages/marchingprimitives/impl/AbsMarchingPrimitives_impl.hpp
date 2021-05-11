#ifndef CIE_MESHKERNEL_ABS_MARCHING_PRIMITIVES_IMPL_HPP
#define CIE_MESHKERNEL_ABS_MARCHING_PRIMITIVES_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/maths/inc/bit.hpp"
#include "cieutils/packages/concurrency/inc/ParallelFor.hpp"

#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"


namespace cie::mesh {


template <concepts::CSGObject TargetType>
AbsMarchingPrimitives<TargetType>::AbsMarchingPrimitives( typename AbsMarchingPrimitives<TargetType>::target_ptr p_target,
                                                          const typename AbsMarchingPrimitives<TargetType>::edge_table& r_edgeTable,
                                                          const typename AbsMarchingPrimitives<TargetType>::connectivity_table& r_connectivityTable,
                                                          typename AbsMarchingPrimitives<TargetType>::output_functor outputFunctor ) :
    _p_target( p_target ),
    _edgeTable( r_edgeTable ),
    _connectivityTable( r_connectivityTable ),
    _outputFunctor( outputFunctor )
{
}


template <concepts::CSGObject TargetType>
void
AbsMarchingPrimitives<TargetType>::execute( mp::ThreadPoolPtr p_threadPool )
{
    CIE_BEGIN_EXCEPTION_TRACING

    Size numberOfPrimitivesToProcess  = this->numberOfRemainingPrimitives();
    Size numberOfVerticesPerPrimitive = this->primitiveVertexCount();

    auto job = [=,this]( Size primitiveIndex ) -> void
    {
        Size configurationIndex = 0;

        // Evaluate target and build configuration
        for ( Size vertexIndex=0; vertexIndex<numberOfVerticesPerPrimitive; ++vertexIndex )
            if ( !this->_p_target->at( this->getVertex(primitiveIndex,vertexIndex) ) )
                configurationIndex = utils::flipBit( configurationIndex, vertexIndex );

        // Find surface primitive constructor map
        const auto& r_edgeSets = this->_connectivityTable[configurationIndex];

        // Get edge indices for each output vertex
        typename AbsMarchingPrimitives<TargetType>::output_arguments outputArguments;

        for ( const auto& r_edgeSet : r_edgeSets )
        {
            for ( Size vertexIndex=0; vertexIndex<r_edgeSet.size(); ++vertexIndex )
                outputArguments[vertexIndex] = this->_edgeTable[ r_edgeSet[vertexIndex] ];

            this->_outputFunctor( primitiveIndex, outputArguments );
        }
    };

    if ( p_threadPool )
        mp::ParallelFor().setPool(p_threadPool)(
            numberOfPrimitivesToProcess,
            job
        );
    else
        for ( Size primitiveIndex=0; primitiveIndex<numberOfPrimitivesToProcess; ++primitiveIndex )
            job( primitiveIndex );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::CSGObject TargetType>
inline void
AbsMarchingPrimitives<TargetType>::setOutputFunctor( typename AbsMarchingPrimitives<TargetType>::output_functor outputFunctor )
{
    this->_outputFunctor = outputFunctor;
}


template <concepts::CSGObject TargetType>
void
AbsMarchingPrimitives<TargetType>::checkIfInitialized() const
{
    CIE_CHECK_POINTER( this->_p_target )
    CIE_CHECK( !this->_r_connectivityTable.empty(), "Empty connectivity table!" )
    CIE_CHECK( this->_outputFunctor != nullptr, "Unset output functor!" )
}


} // namespace cie::mesh


#endif