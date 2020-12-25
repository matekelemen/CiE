#ifndef CIE_CSG_PRIMITIVES_BOX_IMPL_HPP
#define CIE_CSG_PRIMITIVES_BOX_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::csg {


/* --- Box --- */

template < Size Dimension,
           concepts::NumericType CoordinateType >
Box<Dimension,CoordinateType>::Box( typename Box<Dimension,CoordinateType>::point_type& r_base,
                                    typename Box<Dimension,CoordinateType>::point_type& r_lengths ) :
    Box<Dimension,CoordinateType>( r_base, r_lengths )
{
    #ifdef CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS
    bool positiveEdgeLengths = true;
    for ( const auto& length : r_lengths )
        if ( length < 0 )
        {
            positiveEdgeLengths = false;
            break;
        }

    CIE_RUNTIME_GEOMETRY_CHECK(
        positiveEdgeLengths == true,
        "Edge lengths of a box must be non-negative" 
        )
    #endif
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
template <class ContainerType1, class ContainerType2>
requires concepts::ClassContainer<ContainerType1,CoordinateType>
         && concepts::ClassContainer<ContainerType2,CoordinateType>
Box<Dimension,CoordinateType>::Box( const ContainerType1& r_base,
                                    const ContainerType2& r_lengths  )
{
    CIE_OUT_OF_RANGE_CHECK( r_base.size() == Dimension )
    CIE_OUT_OF_RANGE_CHECK( r_lengths.size() == Dimension )

    #ifdef CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS
    bool positiveEdgeLengths = true;
    for ( const auto& length : r_lengths )
        if ( length < 0 )
        {
            positiveEdgeLengths = false;
            break;
        }

    CIE_RUNTIME_GEOMETRY_CHECK(
        positiveEdgeLengths == true,
        "Edge lengths of a box must be non-negative" 
        )
    #endif

    std::copy(  r_base.begin(),
                r_base.end(),
                _base.begin() );
    std::copy(  r_lengths.begin(),
                r_lengths.end(),
                _lengths.begin() );
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
Box<Dimension,CoordinateType>::Box() :
    Box<Dimension,CoordinateType>( detail::makeOrigin<Dimension,CoordinateType>(),
                                   detail::makeOrigin<Dimension,CoordinateType>() )
{
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline Bool
Box<Dimension,CoordinateType>::isDegenerate() const
{
    Bool degenerate = false;
    for (const auto& r_length : _lengths)
        if ( r_length < 1e-16 )
        {
            degenerate = true;
            break;
        }

    return degenerate;
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline const typename Box<Dimension,CoordinateType>::point_type&
Box<Dimension,CoordinateType>::base() const
{
    return _base;
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline const typename Box<Dimension,CoordinateType>::point_type&
Box<Dimension,CoordinateType>::lengths() const
{
    return _lengths;
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline typename Box<Dimension,CoordinateType>::point_type&
Box<Dimension,CoordinateType>::base()
{
    return _base;
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline typename Box<Dimension,CoordinateType>::point_type&
Box<Dimension,CoordinateType>::lengths()
{
    return _lengths;
}



/* --- boolean::Box --- */

namespace boolean {


template <Size Dimension, concepts::NumericType CoordinateType>
Box<Dimension,CoordinateType>::Box( const typename Box<Dimension,CoordinateType>::point_type& r_base,
                                    const typename Box<Dimension,CoordinateType>::point_type& r_lengths ) :
    cie::csg::Box<Dimension,CoordinateType>( r_base, r_lengths )
{
}


template <  Size Dimension,
            concepts::NumericType CoordinateType   >
template <class ContainerType1, class ContainerType2>
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType>
Box<Dimension,CoordinateType>::Box( const ContainerType1& r_base,
                                    const ContainerType2& r_lengths  ) :
    cie::csg::Box<Dimension,CoordinateType>( r_base, r_lengths )
{
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
Bool
Box<Dimension,CoordinateType>::at( const typename Box<Dimension,CoordinateType>::point_type& r_point ) const
{
    CIE_OUT_OF_RANGE_CHECK( r_point.size() == Dimension )

    auto it_base        = this->_base.begin();
    auto it_length      = this->_lengths.begin();
    auto it_pointEnd    = r_point.end();

    for (auto it_point=r_point.begin(); it_point!=it_pointEnd; ++it_point,++it_base,++it_length)
        if ( (*it_point<(*it_base)) == (*it_point<(*it_base + (*it_length))) )
            return false;
    return true;
}


} // namespace boolean


} // namespace cie::csg


#endif