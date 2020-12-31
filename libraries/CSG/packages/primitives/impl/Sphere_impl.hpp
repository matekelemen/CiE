#ifndef CIE_CSG_SPHERE_IMPL_HPP
#define CIE_CSG_SPHERE_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
Sphere<Dimension,CoordinateType>::Sphere( const typename Sphere<Dimension,CoordinateType>::point_type& r_center,
                                          CoordinateType radius ) :
    _center( r_center ),
    _radius( radius )
{
    CIE_RUNTIME_GEOMETRY_CHECK(
        this->_radius >= 0.0,
        "Sphere radius must be non-negative"
    )
}


template <Size Dimension, concepts::NumericType CoordinateType>
template <class ContainerType>
requires concepts::ClassContainer<ContainerType,CoordinateType>
Sphere<Dimension,CoordinateType>::Sphere( const ContainerType& r_center,
                                          CoordinateType radius )
{
    CIE_OUT_OF_RANGE_CHECK(
        r_center.size() == Dimension
    )

    CIE_RUNTIME_GEOMETRY_CHECK(
        radius >= 0.0,
        "Sphere radius must be non-negative"
    )

    this->_radius = radius;
    utils::resize( this->_center, Dimension );

    std::copy( r_center.begin(),
               r_center.end(),
               this->_center.begin() );
}


template <Size Dimension, concepts::NumericType CoordinateType>
inline Bool
Sphere<Dimension,CoordinateType>::isDegenerate() const
{
    return this->_radius == 0;
}


template <Size Dimension, concepts::NumericType CoordinateType>
inline typename Sphere<Dimension,CoordinateType>::point_type&
Sphere<Dimension,CoordinateType>::center()
{
    return this->_center;
}


template <Size Dimension, concepts::NumericType CoordinateType>
inline const typename Sphere<Dimension,CoordinateType>::point_type&
Sphere<Dimension,CoordinateType>::center() const
{
    return this->_center;
}


template <Size Dimension, concepts::NumericType CoordinateType>
inline CoordinateType&
Sphere<Dimension,CoordinateType>::radius()
{
    return this->_radius;
}


template <Size Dimension, concepts::NumericType CoordinateType>
inline const CoordinateType&
Sphere<Dimension,CoordinateType>::radius() const
{
    return this->_radius;
}



/* --- BOOLEAN SPHERE --- */

namespace boolean {


template <Size Dimension, concepts::NumericType CoordinateType>
Sphere<Dimension,CoordinateType>::Sphere( const typename Sphere<Dimension,CoordinateType>::point_type& r_center,
                                          CoordinateType radius ) :
    cie::csg::Sphere<Dimension,CoordinateType>( r_center, radius )
{
}


template <Size Dimension, concepts::NumericType CoordinateType>
template <class ContainerType>
requires concepts::ClassContainer<ContainerType,CoordinateType>
Sphere<Dimension,CoordinateType>::Sphere( const ContainerType& r_center,
                                          CoordinateType radius ) :
    cie::csg::Sphere<Dimension,CoordinateType>( r_center, radius )
{
}


template <Size Dimension, concepts::NumericType CoordinateType>
inline Bool
Sphere<Dimension,CoordinateType>::at( const typename Sphere<Dimension,CoordinateType>::point_type& r_point ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    CoordinateType distance = 0;
    CoordinateType tmp = 0;
    
    for ( Size dim=0; dim<Dimension; ++dim )
    {
        tmp = this->_center[dim] - r_point[dim];
        distance += tmp*tmp;
    }

    return distance <= this->_radius;

    CIE_END_EXCEPTION_TRACING
}


} // namespace boolean


} // namespace cie::csg

#endif