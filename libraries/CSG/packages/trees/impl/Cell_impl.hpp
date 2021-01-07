#ifndef CIE_CSG_TREES_CELL_IMPL_HPP
#define CIE_CSG_TREES_CELL_IMPL_HPP


namespace cie::csg {


/* --- CubeCell --- */

// 2 children per dimension
template <concepts::Cube PrimitiveType>
const GridIndexConverter<PrimitiveType::dimension>
    CubeCell<PrimitiveType>::_childIndexConverter(2);


template <concepts::Cube PrimitiveType>
CubeCell<PrimitiveType>::CubeCell(   const typename CubeCell<PrimitiveType>::point_type& base, 
                                typename CubeCell<PrimitiveType>::coordinate_type length ) :
    CubeCell<PrimitiveType>::cell_base_type( base, length )
{
}


template <concepts::Cube PrimitiveType>
inline typename CubeCell<PrimitiveType>::primitive_constructor_container_ptr
CubeCell<PrimitiveType>::split_internal( const typename CubeCell<PrimitiveType>::point_type& point )
{
    auto p_constructorArgumentsContainer = typename CubeCell<PrimitiveType>::primitive_constructor_container_ptr(
        new typename CubeCell<PrimitiveType>::primitive_constructor_container
    );

    typename CubeCell<PrimitiveType>::point_type tempBase;
    Size numberOfChildren = intPow(Size(2),CubeCell<PrimitiveType>::dimension);

    for (Size childIndex=0; childIndex < numberOfChildren; ++childIndex)
    {
        for (Size dim=0; dim<CubeCell<PrimitiveType>::dimension; ++dim)
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



template <concepts::Cube PrimitiveType>
inline typename CubeCell<PrimitiveType>::primitive_constructor_container_ptr
CubeCell<PrimitiveType>::split( const typename CubeCell<PrimitiveType>::point_type& r_point )
{
    return AbsCell<PrimitiveType>::split(r_point);
}


template <concepts::Cube PrimitiveType>
inline typename CubeCell<PrimitiveType>::primitive_constructor_container_ptr
CubeCell<PrimitiveType>::split( )
{
    return this->split_internal( typename CubeCell<PrimitiveType>::point_type() );
}


/* --- BoxCell --- */

// 2 children per dimension
template <concepts::Box PrimitiveType>
const GridIndexConverter<PrimitiveType::dimension> 
    BoxCell<PrimitiveType>::_childIndexConverter(2);


template <concepts::Box PrimitiveType>
BoxCell<PrimitiveType>::BoxCell(  const typename BoxCell<PrimitiveType>::point_type& base, 
                            const typename BoxCell<PrimitiveType>::point_type& lengths ) :
    BoxCell<PrimitiveType>::cell_base_type( base, lengths )
{
}


template <concepts::Box PrimitiveType>
inline typename BoxCell<PrimitiveType>::primitive_constructor_container_ptr
BoxCell<PrimitiveType>::split_internal( const typename BoxCell<PrimitiveType>::point_type& point )
{
    auto p_constructorArgumentsContainer = typename BoxCell<PrimitiveType>::primitive_constructor_container_ptr(
        new typename BoxCell<PrimitiveType>::primitive_constructor_container
    );

    typename BoxCell<PrimitiveType>::point_type tempBase, tempLengths;
    Size numberOfChildren = intPow(Size(2),BoxCell<PrimitiveType>::dimension);

    for (Size childIndex=0; childIndex < numberOfChildren; ++childIndex)
    {
        for (Size dim=0; dim<BoxCell<PrimitiveType>::dimension; ++dim)
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