#ifndef CIE_CSG_CELL_IMPL_HPP
#define CIE_CSG_CELL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/maths/inc/power.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

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
inline typename AbsCell<PrimitiveType>::primitive_constructor_container_ptr
AbsCell<PrimitiveType>::split( const typename AbsCell<PrimitiveType>::point_type& point )
{
    return this->split_internal(point);
}


// ---------------------------------------------------------
// BASIC CELL TYPES
// ---------------------------------------------------------

/* --- CubeCell --- */

// 2 children per dimension
template <concepts::PrimitiveType CubeType>
const GridIndexConverter<CubeType::dimension>
    CubeCell<CubeType>::_childIndexConverter(2);


template <concepts::PrimitiveType CubeType>
CubeCell<CubeType>::CubeCell(   const typename CubeCell<CubeType>::point_type& base, 
                                typename CubeCell<CubeType>::coordinate_type length ) :
    CubeCell<CubeType>::cell_base_type( base, length )
{
}


template <concepts::PrimitiveType CubeType>
inline typename CubeCell<CubeType>::primitive_constructor_container_ptr
CubeCell<CubeType>::split_internal( const typename CubeCell<CubeType>::point_type& point )
{
    auto p_constructorArgumentsContainer = typename CubeCell<CubeType>::primitive_constructor_container_ptr(
        new typename CubeCell<CubeType>::primitive_constructor_container
    );

    typename CubeCell<CubeType>::point_type tempBase;

    for (Size childIndex=0; childIndex < intPow(Size(2),CubeCell<CubeType>::dimension); ++childIndex)
    {
        for (Size dim=0; dim<CubeCell<CubeType>::dimension; ++dim)
        {
            if (_childIndexConverter.convert(childIndex)[dim] == 0)
                tempBase[dim]   = this->_base[dim];
            else
                tempBase[dim]   = this->_base[dim] + this->_length/2.0;
        }

        p_constructorArgumentsContainer->emplace_back( tempBase, this->_length/2.0 );
    }

    return p_constructorArgumentsContainer;
}



template <concepts::PrimitiveType CubeType>
inline typename CubeCell<CubeType>::primitive_constructor_container_ptr
CubeCell<CubeType>::split( const typename CubeCell<CubeType>::point_type& r_point )
{
    return AbsCell<CubeType>::split(r_point);
}


template <concepts::PrimitiveType CubeType>
inline typename CubeCell<CubeType>::primitive_constructor_container_ptr
CubeCell<CubeType>::split( )
{
    return this->split_internal( typename CubeCell<CubeType>::point_type() );
}


/* --- BoxCell --- */

// 2 children per dimension
template <concepts::PrimitiveType BoxType>
const GridIndexConverter<BoxType::dimension> 
    BoxCell<BoxType>::_childIndexConverter(2);


template <concepts::PrimitiveType BoxType>
BoxCell<BoxType>::BoxCell(  const typename BoxCell<BoxType>::point_type& base, 
                            const typename BoxCell<BoxType>::point_type& lengths ) :
    BoxCell<BoxType>::cell_base_type( base, lengths )
{
}


template <concepts::PrimitiveType BoxType>
inline typename BoxCell<BoxType>::primitive_constructor_container_ptr
BoxCell<BoxType>::split_internal( const typename BoxCell<BoxType>::point_type& point )
{
    auto p_constructorArgumentsContainer = typename BoxCell<BoxType>::primitive_constructor_container_ptr(
        new typename BoxCell<BoxType>::primitive_constructor_container
    );

    typename BoxCell<BoxType>::point_type tempBase, tempLengths;

    for (Size childIndex=0; childIndex < intPow(Size(2),BoxCell<BoxType>::dimension); ++childIndex)
    {
        for (Size dim=0; dim<BoxCell<BoxType>::dimension; ++dim)
        {
            if (_childIndexConverter.convert(childIndex)[dim] == 0)
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

        p_constructorArgumentsContainer->emplace_back( tempBase, tempLengths );
    }

    return p_constructorArgumentsContainer;
}



} // namespace cie::csg

#endif