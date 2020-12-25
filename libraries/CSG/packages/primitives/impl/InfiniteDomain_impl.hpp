#ifndef CIE_CSG_INFINITE_DOMAIN_IMPL_HPP
#define CIE_CSG_INFINITE_DOMAIN_IMPL_HPP


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
inline Bool
InfiniteDomain<Dimension,CoordinateType>::at( const typename InfiniteDomain<Size,CoordinateType>::point_type& r_point ) const
{
    return true;
}


} // namespace cie::csg


#endif