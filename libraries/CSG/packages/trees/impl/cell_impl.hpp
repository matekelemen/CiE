#ifndef CIE_CSG_CELL_IMPL_HPP
#define CIE_CSG_CELL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/indexconverter.hpp"

// --- STL Includes ---
#include <iostream>

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


template <Size dimension, class SelfType, concepts::NumericType CoordinateType>
template <class ContainerType>
CubeCellTemplate<dimension,SelfType,CoordinateType>::CubeCellTemplate(  const ContainerType& base, 
                                                                        CoordinateType length )
requires concepts::ClassContainer<ContainerType,CoordinateType> :
    CubeCellTemplate<dimension,SelfType,CoordinateType>::cell_base_type( base, length )
{
}


template <Size dimension, class SelfType, concepts::NumericType CoordinateType>
typename CubeCellTemplate<dimension,SelfType,CoordinateType>::child_container_type&
CubeCellTemplate<dimension,SelfType,CoordinateType>::split_internal( const typename CubeCellTemplate<dimension,SelfType,CoordinateType>::point_type& point )
{
    if (this->_children.size() != 0)
    {
        std::cerr << "Overwriting existing children!\n";
        this->_children.clear();
    }

    this->_children.resize( intPow(2,dimension) );
    typename CubeCellTemplate<dimension,SelfType,CoordinateType>::point_type tempBase;
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

        this->_children[childIndex] = std::make_shared<CubeCellTemplate<dimension,SelfType,CoordinateType>>( tempBase, this->_length / 2.0 );
    }
    return this->_children;
}


template <Size dimension, class SelfType, concepts::NumericType CoordinateType>
typename CubeCellTemplate<dimension,SelfType,CoordinateType>::child_container_type&
CubeCellTemplate<dimension,SelfType,CoordinateType>::split( )
{
    return this->split_internal( typename CubeCellTemplate<dimension,SelfType,CoordinateType>::point_type() );
}


template <Size dimension, class SelfType, concepts::NumericType CoordinateType>
template <class ContainerType1, class ContainerType2>
BoxCellTemplate<dimension,SelfType,CoordinateType>::BoxCellTemplate(    const ContainerType1& base, 
                                                                        const ContainerType2& lengths )
requires concepts::ClassContainer<ContainerType1,CoordinateType>
            && concepts::ClassContainer<ContainerType2,CoordinateType> :
    BoxCellTemplate<dimension,SelfType,CoordinateType>::cell_base_type( base, lengths )
{
}


template <Size dimension, class SelfType, concepts::NumericType CoordinateType>
typename BoxCellTemplate<dimension,SelfType,CoordinateType>::child_container_type&
BoxCellTemplate<dimension,SelfType,CoordinateType>::split_internal( const typename BoxCellTemplate<dimension,SelfType,CoordinateType>::point_type& point )
{
    CIE_RUNTIME_GEOMETRY_ASSERT(    this->evaluate(point),
                                    "Cannot split geometry using a point outside it!",
                                    "BoxCellTemplate::split")
    if (this->_children.size() != 0)
    {
        std::cerr << "Overwriting existing children!\n";
        this->_children.clear();
    }

    this->_children.resize( intPow(2,dimension) );
    typename BoxCellTemplate<dimension,SelfType,CoordinateType>::point_type tempBase;
    typename BoxCellTemplate<dimension,SelfType,CoordinateType>::point_type tempLengths;
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

        this->_children[childIndex] = std::make_shared<BoxCellTemplate<dimension,SelfType,CoordinateType>>( tempBase, tempLengths );
    }
    return this->_children;
}


} // namespace boolean



} // namespace cie::csg

#endif