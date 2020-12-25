#ifndef CIE_CSG_UNION_OPERATOR_HPP
#define CIE_CSG_UNION_OPERATOR_HPP

// --- Internal Includes ---
#include "CSG/packages/operators/inc/BinaryOperator.hpp"


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
class Union : public BinaryOperator<Dimension,Bool,CoordinateType>
{
public:
    Union() = default;
    Union( typename Union<Dimension,CoordinateType>::operand_ptr p_lhs,
           typename Union<Dimension,CoordinateType>::operand_ptr p_rhs );
    Union( const Union<Dimension,CoordinateType>& r_rhs ) = default;
    Union<Dimension,CoordinateType>& operator=( const Union<Dimension,CoordinateType>& r_rhs ) = default;

    virtual Bool at( const typename Union<Dimension,CoordinateType>::point_type& r_point ) const override;
};


} // namespace cie::csg

#include "CSG/packages/operators/impl/Union_impl.hpp"

#endif