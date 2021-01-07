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
getVertexOnPrimitive( const PrimitiveType& r_primitive,
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

template <concepts::Box PrimitiveType>
inline typename PrimitiveType::point_type
getVertexOnPrimitive( const PrimitiveType& r_primitive,
                      Size vertexIndex )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto vertex = r_primitive.base();

    for ( Size bitIndex=0; bitIndex<PrimitiveType::dimension; ++bitIndex )
        if ( utils::getBit(vertexIndex,bitIndex) )
            vertex[bitIndex] += r_primitive.lengths()[bitIndex];

    return vertex;

    CIE_END_EXCEPTION_TRACING
}
} // namespace detail


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
UnstructuredMarchingCubes<TargetType,PrimitiveType>::UnstructuredMarchingCubes( typename UnstructuredMarchingCubes<TargetType,PrimitiveType>::target_ptr p_target,
                                                                               typename UnstructuredMarchingCubes<TargetType,PrimitiveType>::primitive_container_ptr p_primitives,
                                                                               typename UnstructuredMarchingCubes<TargetType,PrimitiveType>::output_functor outputFunctor ) :
    UnstructuredMarchingPrimitives<TargetType,PrimitiveType>(
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


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
inline Size
UnstructuredMarchingCubes<TargetType,PrimitiveType>::primitiveVertexCount() const
{
    return intPow(2, UnstructuredMarchingCubes<TargetType,PrimitiveType>::dimension);
}


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
inline typename UnstructuredMarchingCubes<TargetType,PrimitiveType>::point_type
UnstructuredMarchingCubes<TargetType,PrimitiveType>::getVertexOnPrimitive( const PrimitiveType& r_primitive,
                                                                           Size vertexIndex ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return detail::getVertexOnPrimitive( r_primitive, vertexIndex );

    CIE_END_EXCEPTION_TRACING
}


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
StructuredMarchingCubes<TargetType,PrimitiveType>::StructuredMarchingCubes( typename StructuredMarchingCubes<TargetType,PrimitiveType>::target_ptr p_target,
                                                                            const typename StructuredMarchingCubes<TargetType,PrimitiveType>::domain_specifier& r_domain,
                                                                            const typename StructuredMarchingCubes<TargetType,PrimitiveType>::resolution_specifier& r_numberOfPoints,
                                                                            typename StructuredMarchingCubes<TargetType,PrimitiveType>::output_functor outputFunctor ) :
    StructuredMarchingPrimitives<TargetType,PrimitiveType>( p_target,
                                                            r_domain,
                                                            r_numberOfPoints,
                                                            detail::cubeEdgeMap,
                                                            detail::marchingCubesConnectivityMap,
                                                            outputFunctor )
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->checkMesh<PrimitiveType>();

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


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
inline Size
StructuredMarchingCubes<TargetType,PrimitiveType>::getGlobalVertexIndex( Size primitiveIndex,
                                                                         Size vertexIndex ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto primitiveResolution = this->_numberOfPoints;
    for ( auto& resolution : primitiveResolution )
        resolution -= 1;

    auto primitiveCartesianIndex = this->getCartesianIndex( primitiveIndex, primitiveResolution );

    Size globalIndex               = 0;
    Size numberOfVerticesInSubMesh = 1;

    for ( Size dim=0; dim<StructuredMarchingCubes<TargetType,PrimitiveType>::dimension; ++dim )
    {
        // Index contribution from the local origin (base of the primitive)
        // + index contribution from the local vertex index

        globalIndex += ( utils::getBit(vertexIndex,dim) + primitiveCartesianIndex[dim]) * numberOfVerticesInSubMesh;

        numberOfVerticesInSubMesh *= this->_numberOfPoints[dim];
    }

    return globalIndex;

    CIE_END_EXCEPTION_TRACING
}


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
inline Size
StructuredMarchingCubes<TargetType,PrimitiveType>::primitiveVertexCount() const
{
    return intPow(2, StructuredMarchingCubes<TargetType,PrimitiveType>::dimension);
}


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
inline Size
StructuredMarchingCubes<TargetType,PrimitiveType>::numberOfRemainingPrimitives() const
{
    return std::accumulate(
        this->_numberOfPoints.begin(),
        this->_numberOfPoints.end(),
        1.0,
        []( Size lhs, Size rhs ) -> Size { return lhs * (rhs-1); }
    );
}


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
typename StructuredMarchingCubes<TargetType,PrimitiveType>::point_type
StructuredMarchingCubes<TargetType,PrimitiveType>::getVertexOnPrimitive( const PrimitiveType& r_primitive,
                                                                         Size vertexIndex ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return detail::getVertexOnPrimitive( r_primitive, vertexIndex );

    CIE_END_EXCEPTION_TRACING
}


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
template <concepts::Cube T>
void
StructuredMarchingCubes<TargetType,PrimitiveType>::checkMesh() const
{
    for ( auto meshEdgeLength : this->_meshEdgeLengths )
        CIE_CHECK( 
            std::abs(meshEdgeLength - this->_meshEdgeLengths[0]) < 1e-15,
            "Invalid domain for StructuredMarchingCubes, consider using StructuredMarchingBoxes" )
}


template < concepts::CSGObject TargetType,
           class PrimitiveType >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
template <concepts::Box T>
void
StructuredMarchingCubes<TargetType,PrimitiveType>::checkMesh() const
{
}


} // namespace cie::mesh


#endif