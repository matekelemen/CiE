#ifndef CIE_CSG_PRIMITIVES_IMPL_HPP
#define CIE_CSG_PRIMITIVES_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- STL Includes ---
#include <algorithm>

namespace cie {
namespace csg {


// ---------------------------------------------------------
// ABSTRACT PRIMITIVES
// ---------------------------------------------------------
template <  Size N, 
            concepts::NumericType CoordinateType   >
template <class ContainerType>
Cube<N,CoordinateType>::Cube(   const ContainerType& base,
                                CoordinateType length  )
    requires concepts::ClassContainer<ContainerType,CoordinateType> :
    _length(length)
{
    CIE_OUT_OF_RANGE_ASSERT( base.size() == N, "Cube::Cube" )

    std::copy(  base.begin(),
                base.end(),
                _base.begin() );
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
const typename Cube<N,CoordinateType>::point_type&
Cube<N,CoordinateType>::base() const
{
    return _base;
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
const typename Cube<N,CoordinateType>::coordinate_type&
Cube<N,CoordinateType>::length() const
{
    return _length;
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
typename Cube<N,CoordinateType>::point_type&
Cube<N,CoordinateType>::base()
{
    return _base;
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
typename Cube<N,CoordinateType>::coordinate_type&
Cube<N,CoordinateType>::length()
{
    return _length;
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
template <class ContainerType1, class ContainerType2>
Box<N,CoordinateType>::Box( const ContainerType1& base,
                            const ContainerType2& lengths  )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType>
{
    CIE_OUT_OF_RANGE_ASSERT( base.size() == N, "Box::Box" )
    CIE_OUT_OF_RANGE_ASSERT( lengths.size() == N, "Box::Box" )

    std::copy(  base.begin(),
                base.end(),
                _base.begin() );
    std::copy(  lengths.begin(),
                lengths.end(),
                _lengths.begin() );
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
const typename Box<N,CoordinateType>::point_type&
Box<N,CoordinateType>::base() const
{
    return _base;
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
Box<N,CoordinateType>::base()
{
    return _base;
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
template <class ContainerType>
CSGCube<N,CoordinateType>::CSGCube( const ContainerType& base,
                                    CoordinateType length  )
    requires concepts::ClassContainer<ContainerType,CoordinateType> :
        Cube<N,CoordinateType>(base,length)
{
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
Bool
CSGCube<N,CoordinateType>::at( const typename CSGCube<N,CoordinateType>::point_type& point ) const
{
    CIE_OUT_OF_RANGE_ASSERT( point.size() == N, "CSGCube::at" )

    auto baseIt = this->_base.begin();
    for (auto pointIt=point.begin(); pointIt!=point.end(); ++pointIt,++baseIt)
        if ( ((*pointIt) < (*baseIt)) == ((*pointIt) < ((*baseIt) + this->_length)) )
            return false;
    return true;
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
template <class ContainerType1, class ContainerType2>
CSGBox<N,CoordinateType>::CSGBox(   const ContainerType1& base,
                                    const ContainerType2& lengths  )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType> :
                    Box<N,CoordinateType>(base,lengths)
{
}


template <  Size N, 
            concepts::NumericType CoordinateType   >
Bool
CSGBox<N,CoordinateType>::at( const typename CSGBox<N,CoordinateType>::point_type& point ) const
{
    CIE_OUT_OF_RANGE_ASSERT( point.size() == N, "CSGBox::at" )

    auto baseIt     = this->_base.begin();
    auto lengthIt   = this->_lengths.begin();
    for (auto pointIt=point.begin(); pointIt!=point.end(); ++pointIt,++baseIt,++lengthIt)
        if ( (*pointIt<(*baseIt)) == (*pointIt<(*baseIt + (*lengthIt))) )
            return false;
    return true;
}

}


}
}

#endif