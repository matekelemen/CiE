#ifndef CIE_CSG_TREES_CELL_IMPL_HPP
#define CIE_CSG_TREES_CELL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/maths/inc/power.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/CartesianIndexConverter.hpp"


namespace cie::csg {


/* --- Cube cell --- */

// 2 children per dimension
template <concepts::Cube PrimitiveType>
const CartesianIndexConverter<PrimitiveType::dimension>
    Cell<PrimitiveType>::_childIndexConverter(2);


template <concepts::Cube PrimitiveType>
Cell<PrimitiveType>::Cell( const typename Cell<PrimitiveType>::point_type& base, 
                           typename Cell<PrimitiveType>::coordinate_type length ) :
    Cell<PrimitiveType>::cell_base_type( base, length )
{
}


template <concepts::Cube PrimitiveType>
inline typename Cell<PrimitiveType>::primitive_constructor_container_ptr
Cell<PrimitiveType>::split_internal( const typename Cell<PrimitiveType>::point_type& point )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_constructorArgumentsContainer = typename Cell<PrimitiveType>::primitive_constructor_container_ptr(
        new typename Cell<PrimitiveType>::primitive_constructor_container
    );

    typename Cell<PrimitiveType>::point_type tempBase;
    Size numberOfChildren = intPow(Size(2),Cell<PrimitiveType>::dimension);

    for (Size childIndex=0; childIndex < numberOfChildren; ++childIndex)
    {
        for (Size dim=0; dim<Cell<PrimitiveType>::dimension; ++dim)
        {
            if (_childIndexConverter.convert(childIndex)[dim] == 0)
                tempBase[dim]   = this->_base[dim];
            else
                tempBase[dim]   = this->_base[dim] + this->_length/2.0;
        }

        p_constructorArgumentsContainer->emplace_back( tempBase, this->_length/2.0 );
    }

    return p_constructorArgumentsContainer;

    CIE_END_EXCEPTION_TRACING
}



template <concepts::Cube PrimitiveType>
inline typename Cell<PrimitiveType>::primitive_constructor_container_ptr
Cell<PrimitiveType>::split( const typename Cell<PrimitiveType>::point_type& r_point )
{
    return AbsCell<PrimitiveType>::split(r_point);
}


template <concepts::Cube PrimitiveType>
inline typename Cell<PrimitiveType>::primitive_constructor_container_ptr
Cell<PrimitiveType>::split( )
{
    return this->split_internal( typename Cell<PrimitiveType>::point_type() );
}


/* --- Box cell --- */

// 2 children per dimension
template <concepts::Box PrimitiveType>
const CartesianIndexConverter<PrimitiveType::dimension> 
    Cell<PrimitiveType>::_childIndexConverter(2);


template <concepts::Box PrimitiveType>
Cell<PrimitiveType>::Cell( const typename Cell<PrimitiveType>::point_type& base, 
                           const typename Cell<PrimitiveType>::point_type& lengths ) :
    Cell<PrimitiveType>::cell_base_type( base, lengths )
{
}


template <concepts::Box PrimitiveType>
inline typename Cell<PrimitiveType>::primitive_constructor_container_ptr
Cell<PrimitiveType>::split_internal( const typename Cell<PrimitiveType>::point_type& point )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_constructorArgumentsContainer = typename Cell<PrimitiveType>::primitive_constructor_container_ptr(
        new typename Cell<PrimitiveType>::primitive_constructor_container
    );

    typename Cell<PrimitiveType>::point_type tempBase, tempLengths;
    Size numberOfChildren = intPow(Size(2),Cell<PrimitiveType>::dimension);

    for (Size childIndex=0; childIndex < numberOfChildren; ++childIndex)
    {
        for (Size dim=0; dim<Cell<PrimitiveType>::dimension; ++dim)
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

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg


#endif