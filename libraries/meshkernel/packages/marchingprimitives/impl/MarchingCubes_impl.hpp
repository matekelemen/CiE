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
MarchingCubes<CoordinateType>::MarchingCubes( typename MarchingCubes<CoordinateType>::target_ptr p_target,
                                              typename MarchingCubes<CoordinateType>::primitive_container_ptr p_primitives,
                                              typename MarchingCubes<CoordinateType>::output_functor outputFunctor ) :
    MarchingPrimitives<TargetType>( p_target,
                                    detail::cubeEdgeMap,
                                    detail::marchingCubesConnectivityMap,
                                    outputFunctor ),
    _p_primitives( p_primitives )
{
}


template <concepts::CSGObject TargetType>
MarchingCubes<CoordinateType>::MarchingCubes( typename MarchingCubes<CoordinateType>::target_ptr p_target,
                                              const typename MarchingCubes<CoordinateType>::domain_specifier& r_domain,
                                              const MarchingCubes<CoordinateType>::resolution_specifier& r_resolution,
                                              typename MarchingCubes<CoordinateType>::output_functor outputFunctor ) :
    MarchingPrimitives<TargetType>( p_target,
                                    detail::cubeEdgeMap,
                                    detail::marchingCubesConnectivityMap,
                                    outputFunctor ),
    _p_primitives()
{
    CIE_BEGIN_EXCEPTION_TRACING

    makeCartesianMesh( _p_primitives,
                        )

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::mesh


#endif