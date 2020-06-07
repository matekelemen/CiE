#ifndef CIE_CSG_CELL_IMPL_HPP
#define CIE_CSG_CELL_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/exceptions.hpp>

// --- Internal Includes ---
#include "spacetreeutils.hpp"

namespace cie::csg {

// ---------------------------------------------------------
// ABSTRACT CELL
// ---------------------------------------------------------
template <Size dimension, concepts::NumericType CoordinateType>
const typename Cell<dimension,CoordinateType>::child_container_type&
Cell<dimension,CoordinateType>::children() const
{
    return _children;
}


template <Size dimension, concepts::NumericType CoordinateType>
typename Cell<dimension,CoordinateType>::child_container_type&
Cell<dimension,CoordinateType>::children()
{
    return _children;
}


template <Size dimension, concepts::NumericType CoordinateType>
Bool
Cell<dimension,CoordinateType>::isinside( const typename Cell<dimension,CoordinateType>::point_type& point )
{
    throw AbstractCallException( "Cell::isInside" );
}


// ---------------------------------------------------------
// PRIMITIVE CELLS
// ---------------------------------------------------------
template <Size dimension, concepts::NumericType CoordinateType>
template <class ContainerType1, class ContainerType2>
BoxCell<dimension,CoordinateType>::BoxCell( const ContainerType1& center, 
                                            const ContainerType2& lengths )
requires concepts::ClassContainer<ContainerType1,CoordinateType>
            && concepts::ClassContainer<ContainerType2,CoordinateType> :
    Box<dimension,CoordinateType>( center, lengths )
{
}


template <Size dimension, concepts::NumericType CoordinateType>
Bool
BoxCell<dimension,CoordinateType>::isInside( const typename BoxCell<dimension,CoordinateType>::point_type& point ) const override
{
    CIE_OUT_OF_RANGE_ASSERT( point.size() == N, "BoxCell::isInside" )

    auto centerIt = this->_center.begin();
    auto lengthIt = this->_lengths.begin();
    for (auto pointIt=point.begin(); pointIt!=point.end(); ++pointIt,++centerIt,++lengthIt)
        if ( (*pointIt<(*centerIt - (*lengthIt)/2.0)) == (*pointIt<(*centerIt + (*lengthIt)/2.0)) )
            return false;
    return true;
}


template <Size dimension, concepts::NumericType CoordinateType>
typename BoxCell<dimension,CoordinateType>::child_container_type&
BoxCell<dimension,CoordinateType>::split( const typename BoxCell<dimension,CoordinateType>::point_type& point ) override
{
    CIE_RUNTIME_GEOMETRY_ASSERT(    isInside(point),
                                    "Cannot split geometry using a point outside it!",
                                    "BoxCell::split")
    if (this->_children.size() != 0)
    {
        std::cerr << "Overwriting existing children!\n";
        this->_children.clear();
    }

    _children.resize( intPow(2,dimension) );
    typename BoxCell<dimension,CoordinateType>::point_type tempCenter;
    typename BoxCell<dimension,CoordinateType>::point_type tempLengths;
    auto centerIt       = this->_center.begin();
    auto lengthIt       = this->_lengths.begin();
    auto tempCenterIt   = tempCenter.begin();
    auto tempLengthIt   = tempLengths.begin();

    for (auto childIt=this->_children.begin(); childIt!=this->_children.end(); ++childIt)
        for (auto pointIt=point.begin(); pointIt!=point.end(); ++pointIt,++centerIt,++lengthIt,++tempCenterIt,++tempLengthIt)
        {
            if ( std::distance(this->_children.begin(),childIt) == 0)
            {
                *tempLengthIt = *pointIt - (*centerIt - (*lengthIt)/2.0);
                *tempCenterIt = *pointIt - (*tempLengthIt)/2.0;
            }
            else
            {
                *tempLengthIt = *pointIt + (*centerIt - (*lengthIt)/2.0);
                *tempCenterIt = *pointIt + (*tempLengthIt)/2.0;
            }
        }

}



}

#endif