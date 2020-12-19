#ifndef CIE_CSG_CSG_OBJECT_IMPL_HPP
#define CIE_CSG_CSG_OBJECT_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- STL Includes ---
#include <utility>


namespace cie::csg {

// ---------------------------------------------------------
// ABSTRACT BASE
// ---------------------------------------------------------
template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType    >
template <class ContainerType>
ValueType
CSGObject<N,ValueType,CoordinateType>::evaluate( const ContainerType& point ) const
    requires concepts::ClassContainer<ContainerType,CoordinateType>
{
    CIE_OUT_OF_RANGE_CHECK( point.size() == N )

    typename CSGObject<N,ValueType,CoordinateType>::point_type internalPoint;
    std::copy(  point.begin(),
                point.end(),
                internalPoint.begin() );
    return this->at(internalPoint);
}


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType    >
template <class ContainerType>
ValueType
CSGObject<N,ValueType,CoordinateType>::evaluate( ContainerType&& point ) const
    requires concepts::ClassContainer<ContainerType,CoordinateType>
{
    CIE_OUT_OF_RANGE_CHECK( point.size() == N )
    
    typename CSGObject<N,ValueType,CoordinateType>::point_type internalPoint;
    std::move(  point.begin(),
                point.end(),
                internalPoint.begin() );
    return this->at(internalPoint);
}


/* --- Convenience Functions --- */

namespace detail {

template < Size Dimension,
           concepts::NumericType CoordinateType >
typename CSGTraits<Dimension,CoordinateType>::point_type makeOrigin()
{
    typename CSGTraits<Dimension,CoordinateType>::point_type origin;
    utils::resize( origin, Dimension );

    for ( auto& r_component : origin )
        r_component = 0;

    return origin;
}

} // namespace detail


} // namespace cie::csg

#endif