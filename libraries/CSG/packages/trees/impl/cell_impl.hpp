#ifndef CIE_CSG_CELL_IMPL_HPP
#define CIE_CSG_CELL_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/exceptions.hpp>

// --- Internal Includes ---
#include "../inc/indexconverter.hpp"

namespace cie::csg {

// ---------------------------------------------------------
// ABSTRACT CELL
// ---------------------------------------------------------
template <class PrimitiveType, class SelfType>
template <class ...Args>
AbsCell<PrimitiveType,SelfType>::AbsCell( Args&&... args ) :
    PrimitiveType( std::forward<Args>(args) ... )
{
}


template <class PrimitiveType, class SelfType>
template <concepts::NumericContainer PointType>
typename AbsCell<PrimitiveType,SelfType>::child_container_type& 
AbsCell<PrimitiveType,SelfType>::split( const PointType& point )
{
    typename AbsCell<PrimitiveType,SelfType>::point_type point_internal;
    std::copy( point.begin(), point.end(), point_internal.begin() );
    return this->split_internal(point_internal);
}


template <class PrimitiveType, class SelfType>
typename AbsCell<PrimitiveType,SelfType>::child_container_type& 
AbsCell<PrimitiveType,SelfType>::split( const typename AbsCell<PrimitiveType,SelfType>::point_type& point )
{
    return this->split_internal(point);
}


// ---------------------------------------------------------
// BOOLEAN PRIMITIVE CELLS
// ---------------------------------------------------------
namespace boolean {


template <Size dimension, concepts::NumericType CoordinateType>
template <class ContainerType>
CubeCell<dimension,CoordinateType>::CubeCell(   const ContainerType& base, 
                                                CoordinateType length )
requires concepts::ClassContainer<ContainerType,CoordinateType> :
    CubeCell<dimension,CoordinateType>::cell_base_type( base, length )
{
}


template <Size dimension, concepts::NumericType CoordinateType>
typename CubeCell<dimension,CoordinateType>::child_container_type&
CubeCell<dimension,CoordinateType>::split_internal( const typename CubeCell<dimension,CoordinateType>::point_type& point )
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
    return this->split_internal( typename CubeCell<dimension,CoordinateType>::point_type() );
}


template <Size dimension, concepts::NumericType CoordinateType>
template <class ContainerType1, class ContainerType2>
BoxCell<dimension,CoordinateType>::BoxCell( const ContainerType1& base, 
                                            const ContainerType2& lengths )
requires concepts::ClassContainer<ContainerType1,CoordinateType>
            && concepts::ClassContainer<ContainerType2,CoordinateType> :
    BoxCell<dimension,CoordinateType>::cell_base_type( base, lengths )
{
}


template <Size dimension, concepts::NumericType CoordinateType>
typename BoxCell<dimension,CoordinateType>::child_container_type&
BoxCell<dimension,CoordinateType>::split_internal( const typename BoxCell<dimension,CoordinateType>::point_type& point )
{
    CIE_RUNTIME_GEOMETRY_ASSERT(    this->evaluate(point),
                                    "Cannot split geometry using a point outside it!",
                                    "BoxCell::split")
    if (this->_children.size() != 0)
    {
        std::cerr << "Overwriting existing children!\n";
        this->_children.clear();
    }

    this->_children.resize( intPow(2,dimension) );
    typename BoxCell<dimension,CoordinateType>::point_type tempBase;
    typename BoxCell<dimension,CoordinateType>::point_type tempLengths;
    SpaceTreeIndexConverter<dimension,2> base2;

    for (Size childIndex=0; childIndex < this->_children.size(); ++childIndex)
    {
        for (Size i=0; i<dimension; ++i)
        {
            if (base2.convert(childIndex)[i] == 0)
            {
                tempLengths[i]  = point[i] - this->_base[i];
                tempBase[i]     = this->_base[i];
            }
            else
            {
                tempLengths[i]  = (this->_base[i] + this->_lengths[i]) - point[i];
                tempBase[i]     = point[i];
            }
        }

        this->_children[childIndex] = std::make_shared<BoxCell<dimension,CoordinateType>>( tempBase, tempLengths );
    }
    return this->_children;
}


}



}

#endif