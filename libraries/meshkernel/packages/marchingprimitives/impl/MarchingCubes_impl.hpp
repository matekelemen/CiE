#ifndef CIE_MESH_KERNEL_MARCHING_CUBES_IMPL_HPP
#define CIE_MESH_KERNEL_MARCHING_CUBES_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/state_iterator.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/connectivitytables.hpp"
#include "meshkernel/packages/structured/inc/cartesianmesh.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::mesh{


template <concepts::CSGObject TargetType>
MarchingCubes<TargetType>::MarchingCubes( typename MarchingCubes<TargetType>::target_ptr p_target,
                                              typename MarchingCubes<TargetType>::primitive_container_ptr p_primitives,
                                              typename MarchingCubes<TargetType>::output_functor outputFunctor ) :
    MarchingPrimitives<TargetType>( p_target,
                                    detail::cubeEdgeMap,
                                    detail::marchingCubesConnectivityMap,
                                    outputFunctor ),
    _p_primitives( p_primitives ),
    _primitiveIndex( 0 )
{
}


template <concepts::CSGObject TargetType>
MarchingCubes<TargetType>::MarchingCubes( typename MarchingCubes<TargetType>::target_ptr p_target,
                                          const typename MarchingCubes<TargetType>::point_type& r_origin,
                                          const typename MarchingCubes<TargetType>::resolution_specifier& r_numberOfPrimitives,
                                          typename MarchingCubes<TargetType>::coordinate_type edgeLength,
                                          typename MarchingCubes<TargetType>::output_functor outputFunctor ) :
    MarchingPrimitives<TargetType>( p_target,
                                    detail::cubeEdgeMap,
                                    detail::marchingCubesConnectivityMap,
                                    outputFunctor ),
    _p_primitives( new typename MarchingCubes<TargetType>::primitive_container ),
    _primitiveIndex( 0 )
{
    CIE_BEGIN_EXCEPTION_TRACING

    makeCartesianMesh<typename MarchingCubes<TargetType>::primitive_type>(
        r_numberOfPrimitives,
        edgeLength,
        r_origin,
        *this->_p_primitives
    );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::CSGObject TargetType>
bool
MarchingCubes<TargetType>::getNextPrimitive( typename MarchingCubes<TargetType>::point_container& r_vertices )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Init
    r_vertices.clear();

    // Terminate if reached the end of the primitives
    if ( this->_p_primitives->size() <= this->_primitiveIndex )
    {
        this->_primitiveIndex = 0;
        return false;
    }

    r_vertices.reserve( 8 );

    std::array<std::array<Size,2>,3> cartesianStates
    {{
        { 0, 1 },
        { 0, 1 },
        { 0, 1 }
    }};
    auto cartesianIterator = utils::makeStateIterator( cartesianStates );

    const auto& r_primitive       = this->_p_primitives->at( this->_primitiveIndex );
    const auto& r_primitiveOrigin = r_primitive.base();
    auto edgeLength               = r_primitive.length();

    for ( Size primitiveIndex=0; primitiveIndex<8; ++primitiveIndex, ++cartesianIterator )
    {
        typename MarchingCubes<TargetType>::point_type vertex = r_primitiveOrigin;
        const auto& r_spatialIndex = *cartesianIterator;

        for ( Size dim=0; dim<3; ++dim )
            vertex[dim] += edgeLength * (*r_spatialIndex[dim]);

        r_vertices.push_back( vertex );
    }

    ++(this->_primitiveIndex);
    return true;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::mesh


#endif