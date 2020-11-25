#ifndef CIE_CSG_PARTITIONING_AABBOX_IMPL_HPP
#define CIE_CSG_PARTITIONING_AABBOX_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::csg {


namespace detail {
template < Size Dimension,
           concepts::NumericType CoordinateType >
typename AABBox<Dimension,CoordinateType>::point_type makeOrigin()
{
    typename AABBox<Dimension,CoordinateType>::point_type origin;
    std::fill_n( origin.begin(),
                 Dimension,
                 0.0 );
    return origin;
}
} // namespace detail


template < Size Dimension,
           concepts::NumericType CoordinateType >
AABBox<Dimension,CoordinateType>::AABBox() :
    boolean::Box<Dimension,Double>( detail::makeOrigin<Dimension,CoordinateType>(), detail::makeOrigin<Dimension,CoordinateType>() )
{
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
AABBox<Dimension,CoordinateType>::AABBox( const typename AABBox<Dimension,CoordinateType>::point_type& r_base,
                           const typename AABBox<Dimension,CoordinateType>::point_type& r_lengths ) :
    boolean::Box<Dimension,Double>( r_base, r_lengths )
{
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline Bool
AABBox<Dimension,CoordinateType>::contains( const AABBox<Dimension,CoordinateType>& r_box ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    // All points inside
    for ( Size dim=0; dim<Dimension; ++dim )
        if ( r_box._base[dim] <= this->_base[dim]
             ||
             this->_base[dim] + this->_lengths[dim] <= r_box._base[dim] + r_box._lengths[dim] )
            return false;

    return true;

    CIE_END_EXCEPTION_TRACING
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline Bool
AABBox<Dimension,CoordinateType>::intersects( const AABBox<Dimension,CoordinateType>& r_box ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    bool hasPointOutside = false;

    // At least one point outside and one inside
    for ( Size dim=0; dim<Dimension; ++dim )
    {
        auto thisMax = this->_base[dim] + this->_lengths[dim];
        auto boxMax  = r_box._base[dim] + r_box._lengths[dim];

        if ( r_box._base[dim] < this->_base[dim] )
        {
            if ( boxMax <= this->_base[dim] )
                return false;
            else if ( boxMax <= thisMax )
                hasPointOutside = true;
        }
        else if ( r_box._base[dim] < thisMax )
        {
            if ( thisMax < boxMax )
                hasPointOutside = true;
        }
        else
            return false;
    }

    return hasPointOutside;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg


#endif