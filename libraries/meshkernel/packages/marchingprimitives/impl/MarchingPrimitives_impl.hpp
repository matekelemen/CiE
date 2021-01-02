#ifndef CIE_MESH_KERNEL_MARCHING_PRIMITIVES_IMPL_HPP
#define CIE_MESH_KERNEL_MARCHING_PRIMITIVES_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/maths/inc/bit.hpp"
#include "cieutils/packages/maths/inc/power.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- STL Includes ---
#include <algorithm>
#include <numeric>


namespace cie::mesh {


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
template <class ...Args>
MarchingPrimitives<TargetType,PrimitiveType>::MarchingPrimitives( Args&&... args ) :
    AbsMarchingPrimitives<TargetType>( std::forward<Args>(args)... )
{
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::UnstructuredMarchingPrimitives( typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::target_ptr p_target,
                                                                                          const typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::edge_table& r_edgeTable,
                                                                                          const typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::connectivity_table& r_connectivityTable,
                                                                                          typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::output_functor outputFunctor,
                                                                                          typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::primitive_container_ptr p_primitives ) :
    MarchingPrimitives<TargetType,PrimitiveType>( p_target,
                                                  r_edgeTable,
                                                  r_connectivityTable,
                                                  outputFunctor ),
    _p_primitives( p_primitives )
{
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
inline typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::point_type
UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::getVertex( Size primitiveIndex,
                                                                     Size vertexIndex )
{
    return this->getVertexOnPrimitive( this->_p_primitives->at( primitiveIndex ),
                                       vertexIndex );
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
inline Size
UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::numberOfRemainingPrimitives() const
{
    return this->_p_primitives->size();
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
inline const UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::primitive_container&
UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::primitives() const
{
    return *this->_p_primitives;
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
StructuredMarchingPrimitives<TargetType,PrimitiveType>::StructuredMarchingPrimitives( typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::target_ptr p_target,
                                                                                      const typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::domain_specifier& r_domain,
                                                                                      const typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::resolution_specifier& r_numberOfPoints,
                                                                                      const typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::edge_table& r_edgeTable,
                                                                                      const typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::connectivity_table& r_connectivityTable,
                                                                                      StructuredMarchingPrimitives<TargetType,PrimitiveType>::output_functor outputFunctor ) :
    MarchingPrimitives<TargetType,PrimitiveType>( p_target,
                                                  r_edgeTable,
                                                  r_connectivityTable,
                                                  outputFunctor ),
    _domain( r_domain ),
    _numberOfPoints( r_numberOfPoints ),
    _meshOrigin(),
    _meshEdgeLengths()
{
    CIE_BEGIN_EXCEPTION_TRACING

    utils::resize( this->_meshOrigin, StructuredMarchingPrimitives<TargetType,PrimitiveType>::dimension );
    utils::resize( this->_meshEdgeLengths, StructuredMarchingPrimitives<TargetType,PrimitiveType>::dimension );

    for ( Size dim=0; dim<StructuredMarchingPrimitives<TargetType,PrimitiveType>::dimension; ++dim )
    {
        this->_meshOrigin[dim] = r_domain[dim].first;

        this->_meshEdgeLengths[dim] = ( r_domain[dim].second - r_domain[dim].first ) / ( r_numberOfPoints[dim] - 1 );
    }

    CIE_END_EXCEPTION_TRACING
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
inline typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::point_type
StructuredMarchingPrimitives<TargetType,PrimitiveType>::getVertex( Size primitiveIndex,
                                                                   Size vertexIndex )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return this->getPointInCartesianMesh(
        this->getGlobalVertexIndex( primitiveIndex, vertexIndex )
    );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
inline typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::point_type
StructuredMarchingPrimitives<TargetType,PrimitiveType>::getPointInCartesianMesh( Size pointIndex ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto point          = this->_meshOrigin;
    auto cartesianIndex = this->getCartesianIndex( pointIndex, this->_numberOfPoints );

    for ( Size dim=0; dim<StructuredMarchingPrimitives<TargetType,PrimitiveType>::dimension; ++dim )
        point[dim] += this->_meshEdgeLengths[dim] * cartesianIndex[dim];

    return point;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::cartesian_index
StructuredMarchingPrimitives<TargetType,PrimitiveType>::getCartesianIndex( Size index,
                                                                           const typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::resolution_specifier& r_resolution ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::cartesian_index cartesianIndex;
    utils::resize( cartesianIndex, StructuredMarchingPrimitives<TargetType,PrimitiveType>::dimension );

    Size numberOfPointsInSubGrid = std::accumulate(
        r_resolution.begin(),
        r_resolution.end(),
        1,
        []( Size lhs, Size rhs ) -> Size { return lhs * rhs; }
    );

    for ( int dim=StructuredMarchingPrimitives<TargetType,PrimitiveType>::dimension-1; dim>=0; --dim )
    {
        numberOfPointsInSubGrid /= r_resolution[dim];

        cartesianIndex[dim] = index / numberOfPointsInSubGrid;
        index -= cartesianIndex[dim] * numberOfPointsInSubGrid;
    }

    CIE_OUT_OF_RANGE_CHECK( index == 0 )

    return cartesianIndex;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::mesh


#endif