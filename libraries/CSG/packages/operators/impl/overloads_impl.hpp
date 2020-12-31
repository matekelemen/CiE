#ifndef CIE_CSG_OVERLOADS_IMPL_HPP
#define CIE_CSG_OVERLOADS_IMPL_HPP


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
CSGObjectPtr<Dimension,Bool,CoordinateType> operator*( CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs,
                                                       CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs )
{
    return CSGObjectPtr<Dimension,Bool,CoordinateType>(
        new Intersection<Dimension,CoordinateType>( p_lhs, p_rhs )
    );
}


template <Size Dimension, concepts::NumericType CoordinateType>
CSGObjectPtr<Dimension,Bool,CoordinateType> operator+( CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs,
                                                       CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs )
{
    return CSGObjectPtr<Dimension,Bool,CoordinateType>(
        new Union<Dimension,CoordinateType>( p_lhs, p_rhs )
    );
}


template <Size Dimension, concepts::NumericType CoordinateType>
CSGObjectPtr<Dimension,Bool,CoordinateType> operator-( CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs,
                                                       CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs )
{
    return CSGObjectPtr<Dimension,Bool,CoordinateType>(
        new Subtraction<Dimension,CoordinateType>( p_lhs, p_rhs )
    );
}


} // namespace cie::csg


#endif