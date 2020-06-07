#ifndef CIE_CSG_PRIMITIVES_IMPL_HPP
#define CIE_CSG_PRIMITIVES_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- STD Includes ---
#include <algorithm>

namespace cie {
namespace csg {


// ---------------------------------------------------------
// ABSTRACT PRIMITIVES
// ---------------------------------------------------------
template <  Size N, 
            concepts::NumericType CoordinateType   >
template <class ContainerType1, class ContainerType2>
Box<N,CoordinateType>::Box( const ContainerType1& center,
                                            const ContainerType2& lengths  )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType>
{
    CIE_OUT_OF_RANGE_TEST( center.size() == N, "Box::Box" )
    CIE_OUT_OF_RANGE_TEST( lengths.size() == N, "Box::Box" )

    std::copy(  center.begin(),
                center.end(),
                _center.begin() );
    std::copy(  lengths.begin(),
                lengths.end(),
                _lengths.begin() );
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
const typename Box<N,CoordinateType>::point_type&
Box<N,CoordinateType>::center() const
{
    return _center;
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
const typename Box<N,CoordinateType>::point_type&
Box<N,CoordinateType>::lengths() const
{
    return _lengths;
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
typename Box<N,CoordinateType>::point_type&
Box<N,CoordinateType>::center()
{
    return _center;
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
typename Box<N,CoordinateType>::point_type&
Box<N,CoordinateType>::lengths()
{
    return _lengths;
}


// ---------------------------------------------------------
// SPECIALIZED BOOLEAN PRIMITIVES
// ---------------------------------------------------------
namespace boolean {

template <  Size N, 
            concepts::NumericType CoordinateType   >
template <class ContainerType1, class ContainerType2>
CSGBox<N,CoordinateType>::CSGBox( const ContainerType1& center,
                            const ContainerType2& lengths  )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType> :
                    Box<N,CoordinateType>(center,lengths)
{
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
Bool
CSGBox<N,CoordinateType>::at( const typename CSGBox<N,CoordinateType>::point_type& point ) const
{
    CIE_OUT_OF_RANGE_TEST( point.size() == N, "CSGBox::at" )

    auto centerIt = this->_center.begin();
    auto lengthIt = this->_lengths.begin();
    for (auto pointIt=point.begin(); pointIt!=point.end(); ++pointIt,++centerIt,++lengthIt)
        if ( (*pointIt<(*centerIt - (*lengthIt)/2.0)) == (*pointIt<(*centerIt + (*lengthIt)/2.0)) )
            return false;
    return true;
}

}


}
}

#endif