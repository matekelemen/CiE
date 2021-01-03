#ifndef CIE_MESH_KERNEL_MARCHING_CUBES_IMPL_HPP
#define CIE_MESH_KERNEL_MARCHING_CUBES_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/state_iterator.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/maths/inc/bit.hpp"

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
    CIE_BEGIN_EXCEPTION_TRACING

    if ( TargetType::dimension == 3 )
    { /* Do nothing: default tables are set for 3 dimensions */ }
    else if ( TargetType::dimension == 2 )
    {
        this->_edgeTable = detail::squareEdgeMap;
        this->_connectivityTable = detail::marchingSquaresConnectivityMap;
    }
    else
        CIE_THROW( NotImplementedException, "MarchingCubes is implemented only in 2 and 3 dimensions" )

    CIE_END_EXCEPTION_TRACING
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


template <concepts::CSGObject TargetType>
StructuredMarchingCubes<TargetType>::StructuredMarchingCubes( typename StructuredMarchingCubes<TargetType>::target_ptr p_target,
                                                              const typename StructuredMarchingCubes<TargetType>::domain_specifier& r_domain,
                                                              const typename StructuredMarchingCubes<TargetType>::resolution_specifier& r_numberOfPoints,
                                                              typename StructuredMarchingCubes<TargetType>::output_functor outputFunctor ) :
    StructuredMarchingPrimitives<TargetType,csg::Cube<TargetType::dimension,typename TargetType::coordinate_type>>( p_target,
                                                                                                                    r_domain,
                                                                                                                    r_numberOfPoints,
                                                                                                                    detail::cubeEdgeMap,
                                                                                                                    detail::marchingCubesConnectivityMap,
                                                                                                                    outputFunctor )
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( auto meshEdgeLength : this->_meshEdgeLengths )
        CIE_CHECK( 
            meshEdgeLength == this->_meshEdgeLengths[0],
            "Invalid domain for StructuredMarchingCubes, consider using StructuredMarchingBoxes" )

    if ( TargetType::dimension == 3 )
    { /* Do nothing: default tables are set for 3 dimensions */ }
    else if ( TargetType::dimension == 2 )
    {
        this->_edgeTable = detail::squareEdgeMap;
        this->_connectivityTable = detail::marchingSquaresConnectivityMap;
    }
    else
        CIE_THROW( NotImplementedException, "MarchingCubes is implemented only in 2 and 3 dimensions" )

    CIE_END_EXCEPTION_TRACING
}


template <concepts::CSGObject TargetType>
inline Size
StructuredMarchingCubes<TargetType>::getGlobalVertexIndex( Size primitiveIndex,
                                                           Size vertexIndex ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto primitiveResolution = this->_numberOfPoints;
    for ( auto& resolution : primitiveResolution )
        resolution -= 1;

    auto primitiveCartesianIndex = this->getCartesianIndex( primitiveIndex, primitiveResolution );

    Size globalIndex               = 0;
    Size numberOfVerticesInSubMesh = 1;

    for ( Size dim=0; dim<StructuredMarchingCubes<TargetType>::dimension; ++dim )
    {
        // Index contribution from the local origin (base of the primitive)
        // + index contribution from the local vertex index

        globalIndex += ( utils::getBit(vertexIndex,dim) + primitiveCartesianIndex[dim]) * numberOfVerticesInSubMesh;

        numberOfVerticesInSubMesh *= this->_numberOfPoints[dim];
    }

    return globalIndex;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::CSGObject TargetType>
inline Size
StructuredMarchingCubes<TargetType>::primitiveVertexCount() const
{
    return intPow(2, StructuredMarchingCubes<TargetType>::dimension);
}


template <concepts::CSGObject TargetType>
inline Size
StructuredMarchingCubes<TargetType>::numberOfRemainingPrimitives() const
{
    return std::accumulate(
        this->_numberOfPoints.begin(),
        this->_numberOfPoints.end(),
        1.0,
        []( Size lhs, Size rhs ) -> Size { return lhs * (rhs-1); }
    );
}


template <concepts::CSGObject TargetType>
typename StructuredMarchingCubes<TargetType>::point_type
StructuredMarchingCubes<TargetType>::getVertexOnPrimitive( const typename StructuredMarchingCubes<TargetType>::primitive_type& r_primitive,
                                                           Size vertexIndex ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return detail::getVertexOnCube( r_primitive, vertexIndex );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::mesh


#endif