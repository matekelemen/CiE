#ifndef CIE_MESH_KERNEL_MARCHING_CUBES_IMPL_HPP
#define CIE_MESH_KERNEL_MARCHING_CUBES_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/state_iterator.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/maths/inc/power.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/connectivitytables.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::mesh {


namespace detail {
template <concepts::Cube PrimitiveType>
inline typename PrimitiveType::point_type
getVertexOnCube( const PrimitiveType& r_primitive,
                 Size vertexIndex )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto vertex = r_primitive.base();

    for ( Size bitIndex=0; bitIndex<PrimitiveType::dimension; ++bitIndex )
        if ( utils::getBit(vertexIndex,bitIndex) )
            vertex[bitIndex] += r_primitive.length();

    return vertex;

    CIE_END_EXCEPTION_TRACING
}
} // namespace detail


template <concepts::CSGObject TargetType>
UnstructuredMarchingCubes<TargetType>::UnstructuredMarchingCubes( typename UnstructuredMarchingCubes<TargetType>::target_ptr p_target,
                                          typename UnstructuredMarchingCubes<TargetType>::primitive_container_ptr p_primitives,
                                          typename UnstructuredMarchingCubes<TargetType>::output_functor outputFunctor ) :
    UnstructuredMarchingPrimitives<TargetType,csg::Cube<TargetType::dimension,typename TargetType::coordinate_type>>(
        p_target,
        detail::cubeEdgeMap,
        detail::marchingCubesConnectivityMap,
        outputFunctor,
        p_primitives )
{
}


template <concepts::CSGObject TargetType>
inline Size
UnstructuredMarchingCubes<TargetType>::primitiveVertexCount() const
{
    return intPow(2, UnstructuredMarchingCubes<TargetType>::dimension);
}


template <concepts::CSGObject TargetType>
inline typename UnstructuredMarchingCubes<TargetType>::point_type
UnstructuredMarchingCubes<TargetType>::getVertexOnPrimitive( const typename UnstructuredMarchingCubes<TargetType>::primitive_type& r_primitive,
                                                             Size vertexIndex ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return detail::getVertexOnCube( r_primitive, vertexIndex );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::mesh


#endif