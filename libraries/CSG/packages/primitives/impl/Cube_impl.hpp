#ifndef CIE_CSG_PRIMITIVES_CUBE_IMPL_HPP
#define CIE_CSG_PRIMITIVES_CUBE_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::csg {


/* --- Cube --- */

template < Size Dimension,
           concepts::NumericType CoordinateType >
Cube<Dimension,CoordinateType>::Cube( const typename Cube<Dimension,CoordinateType>::point_type& r_base,
                                      CoordinateType length ) :
    _base( r_base ),
    _length( length )
{
    CIE_RUNTIME_GEOMETRY_CHECK(
        length >= 0,
        "Edge length of a cube must be non-negative"
    )
}


template < Size Dimension, 
           concepts::NumericType CoordinateType >
template <class ContainerType>
requires concepts::ClassContainer<ContainerType,CoordinateType>
Cube<Dimension,CoordinateType>::Cube( const ContainerType& r_base,
                                      CoordinateType length  ) :
    _length(length)
{
    CIE_OUT_OF_RANGE_CHECK( r_base.size() == Dimension )

    CIE_RUNTIME_GEOMETRY_CHECK(
        length >= 0,
        "Edge length of a cube must be non-negative"
    )

    std::copy(  r_base.begin(),
                r_base.end(),
                _base.begin() );
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline Bool
Cube<Dimension,CoordinateType>::isDegenerate() const
{
    return _length < 1e-16 ? true : false;
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline const typename Cube<Dimension,CoordinateType>::point_type&
Cube<Dimension,CoordinateType>::base() const
{
    return _base;
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline const typename Cube<Dimension,CoordinateType>::coordinate_type&
Cube<Dimension,CoordinateType>::length() const
{
    return _length;
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline typename Cube<Dimension,CoordinateType>::point_type&
Cube<Dimension,CoordinateType>::base()
{
    return _base;
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
inline typename Cube<Dimension,CoordinateType>::coordinate_type&
Cube<Dimension,CoordinateType>::length()
{
    return _length;
}



/* --- boolean::Cube --- */

namespace boolean {


template <Size Dimension, concepts::NumericType CoordinateType>
Cube<Dimension,CoordinateType>::Cube( const typename Cube<Dimension,CoordinateType>::point_type& r_base,
                                      CoordinateType length ) :
    cie::csg::Cube<Dimension,CoordinateType>( r_base, length )
{
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
template <class ContainerType>
requires concepts::ClassContainer<ContainerType,CoordinateType>
Cube<Dimension,CoordinateType>::Cube( const ContainerType& r_base,
                                            CoordinateType length  ) :
    cie::csg::Cube<Dimension,CoordinateType>( r_base, length )
{
}


template < Size Dimension,
           concepts::NumericType CoordinateType >
Bool
Cube<Dimension,CoordinateType>::at( const typename Cube<Dimension,CoordinateType>::point_type& r_point ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_OUT_OF_RANGE_CHECK( r_point.size() == Dimension )

    auto baseIt         = this->_base.begin();
    auto it_pointEnd    = r_point.end();
    for (auto pointIt=r_point.begin(); pointIt!=it_pointEnd; ++pointIt,++baseIt)
        if ( ((*pointIt) < (*baseIt)) == ((*pointIt) < ((*baseIt) + this->_length)) )
            return false;
    return true;

    CIE_END_EXCEPTION_TRACING
}

} // namespace boolean


} // namespace cie::csg


#endif