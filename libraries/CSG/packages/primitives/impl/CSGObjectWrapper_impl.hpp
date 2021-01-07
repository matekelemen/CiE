#ifndef CIE_CSG_OBJECT_WRAPPER_IMPL_HPP
#define CIE_CSG_OBJECT_WRAPPER_IMPL_HPP


namespace cie::csg {


template < Size Dimension,
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
CSGObjectWrapper<Dimension,ValueType,CoordinateType>::CSGObjectWrapper( typename CSGObjectWrapper<Dimension,ValueType,CoordinateType>::functor_type functor ) :
    _functor( functor )
{
}


template < Size Dimension,
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
inline ValueType
CSGObjectWrapper<Dimension,ValueType,CoordinateType>::at( const typename CSGObjectWrapper<Dimension,ValueType,CoordinateType>::point_type& r_point ) const
{
    return this->_functor( r_point );
}


} // namespace cie::csg


#endif