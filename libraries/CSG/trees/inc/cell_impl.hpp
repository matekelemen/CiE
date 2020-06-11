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
AbsCell<dimension,SelfType,CoordinateType>::isInside( const typename AbsCell<dimension,SelfType,CoordinateType>::point_type& point ) const
{
    throw AbstractCallException( "AbsCell::isInside" );
}


// ---------------------------------------------------------
// PRIMITIVE CELLS
// ---------------------------------------------------------
template <Size dimension, concepts::NumericType CoordinateType>
template <class ContainerType>
CubeCell<dimension,CoordinateType>::CubeCell(   const ContainerType& base, 
                                                CoordinateType length )
requires concepts::ClassContainer<ContainerType,CoordinateType> :
    boolean::CSGCube<dimension,CoordinateType>( base, length )
{
}


template <Size dimension, concepts::NumericType CoordinateType>
Bool
CubeCell<dimension,CoordinateType>::isInside( const typename CubeCell<dimension,CoordinateType>::point_type& point ) const
{
    return this->at(point);
}


template <Size dimension, concepts::NumericType CoordinateType>
typename CubeCell<dimension,CoordinateType>::child_container_type&
CubeCell<dimension,CoordinateType>::split( const typename CubeCell<dimension,CoordinateType>::point_type& point )
{
    if (this->_children.size() != 0)
    {
        std::cerr << "Overwriting existing children!\n";
        this->_children.clear();
    }

    this->_children.resize( intPow(2,dimension) );
    typename CubeCell<dimension,CoordinateType>::point_type tempBase;
    SpaceTreeIndexConverter<dimension,2> base2;

    for (Size childIndex=0; childIndex < this->_children.size(); ++childIndex)
    {
        for (Size i=0; i<dimension; ++i)
        {
            if (base2.convert(childIndex)[i] == 0)
                tempBase[i]   = this->_base[i];
            else
                tempBase[i]   = this->_base[i] + this->_length/2.0;
        }

        this->_children[childIndex] = std::make_shared<CubeCell<dimension,CoordinateType>>( tempBase, this->_length / 2.0 );
    }
    return this->_children;
}


template <Size dimension, concepts::NumericType CoordinateType>
typename CubeCell<dimension,CoordinateType>::child_container_type&
CubeCell<dimension,CoordinateType>::split( )
{
    return this->split( typename CubeCell<dimension,CoordinateType>::point_type() );
}


template <Size dimension, concepts::NumericType CoordinateType>
template <class ContainerType1, class ContainerType2>
BoxCell<dimension,CoordinateType>::BoxCell( const ContainerType1& center, 
                                            const ContainerType2& lengths )
requires concepts::ClassContainer<ContainerType1,CoordinateType>
            && concepts::ClassContainer<ContainerType2,CoordinateType> :
    boolean::CSGBox<dimension,CoordinateType>( center, lengths )
{
}


template <Size dimension, concepts::NumericType CoordinateType>
Bool
BoxCell<dimension,CoordinateType>::isInside( const typename BoxCell<dimension,CoordinateType>::point_type& point ) const
{
    return this->at(point);
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
    SpaceTreeIndexConverter<dimension,2> base2;

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