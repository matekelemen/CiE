#ifndef CIE_CSG_CELL_IMPL_HPP
#define CIE_CSG_CELL_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/exceptions.hpp>

// --- Internal Includes ---
#include "indexconverter.hpp"

namespace cie::csg {

// ---------------------------------------------------------
// ABSTRACT CELL
// ---------------------------------------------------------
template <Size dimension, class SelfType, concepts::NumericType CoordinateType>
Bool
Cell<dimension,SelfType,CoordinateType>::isInside( const typename Cell<dimension,SelfType,CoordinateType>::point_type& point ) const
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
BoxCell<dimension,CoordinateType>::isInside( const typename BoxCell<dimension,CoordinateType>::point_type& point ) const
{
    CIE_OUT_OF_RANGE_ASSERT( point.size() == dimension, "BoxCell::isInside" )

    auto centerIt = this->_center.begin();
    auto lengthIt = this->_lengths.begin();
    for (auto pointIt=point.begin(); pointIt!=point.end(); ++pointIt,++centerIt,++lengthIt)
        if ( (*pointIt<(*centerIt - (*lengthIt)/2.0)) == (*pointIt<(*centerIt + (*lengthIt)/2.0)) )
            return false;
    return true;
}


template <Size dimension, concepts::NumericType CoordinateType>
typename BoxCell<dimension,CoordinateType>::child_container_type&
BoxCell<dimension,CoordinateType>::split( const typename BoxCell<dimension,CoordinateType>::point_type& point )
{
    CIE_RUNTIME_GEOMETRY_ASSERT(    isInside(point),
                                    "Cannot split geometry using a point outside it!",
                                    "BoxCell::split")
    if (this->_children.size() != 0)
    {
        std::cerr << "Overwriting existing children!\n";
        this->_children.clear();
    }

    this->_children.resize( intPow(2,dimension) );
    typename BoxCell<dimension,CoordinateType>::point_type tempCenter;
    typename BoxCell<dimension,CoordinateType>::point_type tempLengths;
    SpaceTreeIndexConverter<2,2> base2;

    for (Size childIndex=0; childIndex < this->_children.size(); ++childIndex)
    {
        for (Size i=0; i<dimension; ++i)
        {
            if (base2.convert(childIndex)[i] == 0)
            {
                tempLengths[i]  = point[i] - (this->_center[i] - this->_lengths[i]/2.0);
                tempCenter[i]   = point[i] - tempLengths[i]/2.0;
            }
            else
            {
                tempLengths[i]  = (this->_center[i] + this->_lengths[i]/2.0) - point[i];
                tempCenter[i]   = point[i] + tempLengths[i]/2.0;
            }
        }

        this->_children[childIndex] = std::make_shared<BoxCell<dimension,CoordinateType>>( tempCenter, tempLengths );
    }
    return this->_children;
}



}

#endif