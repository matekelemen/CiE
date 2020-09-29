#ifndef CIE_CSG_CELL_IMPL_HPP
#define CIE_CSG_CELL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/indexconverter.hpp"

// --- STL Includes ---
#include <iostream>

namespace cie::csg {

// ---------------------------------------------------------
// ABSTRACT CELL
// ---------------------------------------------------------

template <concepts::PrimitiveType PrimitiveType>
template <class ...Args>
AbsCell<PrimitiveType>::AbsCell( Args&&... args ) :
    PrimitiveType( std::forward<Args>(args) ... )
{
}


template <concepts::PrimitiveType PrimitiveType>
template <concepts::NumericContainer PointType>
inline typename AbsCell<PrimitiveType>::primitive_constructor_container_ptr
AbsCell<PrimitiveType>::split( const PointType& point )
{
    typename AbsCell<PrimitiveType>::point_type point_internal;
    std::copy( point.begin(), point.end(), point_internal.begin() );
    return this->split_internal(point_internal);
}


template <concepts::PrimitiveType PrimitiveType>
inline typename AbsCell<PrimitiveType>::primitive_constructor_container_ptr
AbsCell<PrimitiveType>::split( const typename AbsCell<PrimitiveType>::point_type& point )
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
inline typename CubeCell<dimension,CoordinateType>::primitive_constructor_container_ptr
CubeCell<dimension,CoordinateType>::split_internal( const typename CubeCell<dimension,CoordinateType>::point_type& point )
{
    auto constructorArgumentsContainerPtr = typename CubeCell<dimension,CoordinateType>::primitive_constructor_container_ptr(
        new typename CubeCell<dimension,CoordinateType>::primitive_constructor_container
    );

    typename CubeCell<dimension,CoordinateType>::point_type tempBase;
    SpaceTreeIndexConverter<dimension,2> base2;

    for (Size childIndex=0; childIndex < intPow(2,dimension); ++childIndex)
    {
        for (Size dim=0; dim<dimension; ++dim)
        {
            if (base2.convert(childIndex)[dim] == 0)
                tempBase[dim]   = this->_base[dim];
            else
                tempBase[dim]   = this->_base[dim] + this->_length/2.0;
        }

        constructorArgumentsContainerPtr->emplace_back( tempBase, this->_length/2.0 );
    }

    return constructorArgumentsContainerPtr;
}


template <Size dimension, concepts::NumericType CoordinateType>
inline typename CubeCell<dimension,CoordinateType>::primitive_constructor_container_ptr
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
inline typename BoxCell<dimension,CoordinateType>::primitive_constructor_container_ptr
BoxCell<dimension,CoordinateType>::split_internal( const typename BoxCell<dimension,CoordinateType>::point_type& point )
{
    CIE_RUNTIME_GEOMETRY_ASSERT(    this->evaluate(point),
                                    "Cannot split geometry using a point outside it!",
                                    "BoxCellTemplate::split")

    auto constructorArgumentsContainerPtr = typename BoxCell<dimension,CoordinateType>::primitive_constructor_container_ptr(
        new typename BoxCell<dimension,CoordinateType>::primitive_constructor_container
    );

    typename BoxCell<dimension,CoordinateType>::point_type tempBase;
    typename BoxCell<dimension,CoordinateType>::point_type tempLengths;
    SpaceTreeIndexConverter<dimension,2> base2;

    for (Size childIndex=0; childIndex < intPow(2,dimension); ++childIndex)
    {
        for (Size dim=0; dim<dimension; ++dim)
        {
            if (base2.convert(childIndex)[dim] == 0)
            {
                tempLengths[dim]  = point[dim] - this->_base[dim];
                tempBase[dim]     = this->_base[dim];
            }
            else
            {
                tempLengths[dim]  = (this->_base[dim] + this->_lengths[dim]) - point[dim];
                tempBase[dim]     = point[dim];
            }
        }

        constructorArgumentsContainerPtr->emplace_back( tempBase, tempLengths );
    }

    return constructorArgumentsContainerPtr;
}


} // namespace boolean



} // namespace cie::csg

#endif