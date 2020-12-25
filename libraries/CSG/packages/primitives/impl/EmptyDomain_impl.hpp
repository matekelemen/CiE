#ifndef CIE_CSG_EMPTY_DOMAIN_IMPL_HPP
#define CIE_CSG_EMPTY_DOMAIN_IMPL_HPP


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
inline Bool
EmptyDomain<Dimension,CoordinateType>::at( const typename EmptyDomain<Size,CoordinateType>::point_type& r_point ) const
{
    return false;
}


} // namespace cie::csg


#endif