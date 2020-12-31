#ifndef CIE_MESH_KERNEL_MARCHING_CUBES_IMPL_HPP
#define CIE_MESH_KERNEL_MARCHING_CUBES_IMPL_HPP

// --- Utility Includes ---
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
    _p_primitives( p_primitives )
{
}


template <concepts::CSGObject TargetType>
MarchingCubes<TargetType>::MarchingCubes( typename MarchingCubes<TargetType>::target_ptr p_target,
                                          const typename MarchingCubes<TargetType>::point_type& r_origin,
                                          const MarchingCubes<TargetType>::resolution_specifier& r_numberOfPrimitives,
                                          MarchingCubes<TargetType>::coordinate_type edgeLength,
                                          typename MarchingCubes<TargetType>::output_functor outputFunctor ) :
    MarchingPrimitives<TargetType>( p_target,
                                    detail::cubeEdgeMap,
                                    detail::marchingCubesConnectivityMap,
                                    outputFunctor ),
    _p_primitives()
{
    CIE_BEGIN_EXCEPTION_TRACING

    makeCartesianMesh( r_numberOfPrimitives,
                       edgeLength,
                       r_origin,
                       *this->_p_primitives );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::CSGObject TargetType>
bool
MarchingCubes<TargetType>::getNextPrimitive( typename MarchingCubes<TargetType>::point_container& r_vertices )
{
    CIE_BEGIN_EXCEPTION_TRACING

    r_primitives.reserve( 8 );
    

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::mesh


#endif