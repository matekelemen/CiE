#ifndef CIE_CSG_OVERLOADS_IMPL_HPP
#define CIE_CSG_OVERLOADS_IMPL_HPP


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
IntersectionPtr<Dimension,CoordinateType> operator*( CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs,
                                                      CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs )
{
    return IntersectionPtr<Dimension,CoordinateType>(
        new Intersection<Dimension,CoordinateType>( p_lhs, p_rhs )
    );
}


template <Size Dimension, concepts::NumericType CoordinateType>
UnionPtr<Dimension,CoordinateType> operator+( CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs,
                                              CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs )
{
    return UnionPtr<Dimension,CoordinateType>(
        new Union<Dimension,CoordinateType>( p_lhs, p_rhs )
    );
}


template <Size Dimension, concepts::NumericType CoordinateType>
SubtractionPtr<Dimension,CoordinateType> operator-( CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs,
                                                    CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs )
{
    return SubtractionPtr<Dimension,CoordinateType>(
        new Subtraction<Dimension,CoordinateType>( p_lhs, p_rhs )
    );
}


} // namespace cie::csg


#endif