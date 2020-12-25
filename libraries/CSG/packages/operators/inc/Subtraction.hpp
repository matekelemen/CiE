#ifndef CIE_CSG_SUBTRACTION_OPERATOR_HPP
#define CIE_CSG_SUBTRACTION_OPERATOR_HPP

// --- Internal Includes ---
#include "CSG/packages/operators/inc/BinaryOperator.hpp"


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
class Subtraction : public BinaryOperator<Dimension,Bool,CoordinateType>
{
public:
    Subtraction() = default;
    Subtraction( typename Subtraction<Dimension,CoordinateType>::operand_ptr p_lhs,
           typename Subtraction<Dimension,CoordinateType>::operand_ptr p_rhs );
    Subtraction( const Subtraction<Dimension,CoordinateType>& r_rhs ) = default;
    Subtraction<Dimension,CoordinateType>& operator=( const Subtraction<Dimension,CoordinateType>& r_rhs ) = default;

    virtual Bool at( const typename Subtraction<Dimension,CoordinateType>::point_type& r_point ) const override;
};


} // namespace cie::csg

#include "CSG/packages/operators/impl/Subtraction_impl.hpp"

#endif