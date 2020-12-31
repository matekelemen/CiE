#ifndef CIE_CSG_BINARY_OPERATOR_HPP
#define CIE_CSG_BINARY_OPERATOR_HPP

// --- Internal Includes ---
#include "CSG/packages/operators/inc/UnaryOperator.hpp"


namespace cie::csg {


template < Size Dimension,
           concepts::CopyConstructible ValueType = Bool,
           concepts::NumericType CoordinateType = Double >
class BinaryOperator : public UnaryOperator<Dimension,ValueType,CoordinateType>
{
public:
    BinaryOperator() = default;
    BinaryOperator( typename BinaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr p_lhs,
                    typename BinaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr p_rhs );
    BinaryOperator( const BinaryOperator<Dimension,ValueType,CoordinateType>& r_rhs ) = default;
    BinaryOperator<Dimension,ValueType,CoordinateType>& operator=( const BinaryOperator<Dimension,ValueType,CoordinateType>& r_rhs ) = default;

    void bindLhs( typename BinaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr p_lhs );

    template <class CSGObjectType, class ...Args>
    void emplaceLhs( Args&&... args )
        requires concepts::DerivedFrom<CSGObjectType,CSGObject<Dimension,ValueType,CoordinateType>>;

    typename BinaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr& lhs();
    const typename BinaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr& lhs() const;

    virtual ValueType at(const typename BinaryOperator<Dimension, ValueType, CoordinateType>::point_type& r_point) const override = 0;

protected:
    typename BinaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr _p_lhs;
};


} // namespace cie::csg

#include "CSG/packages/operators/impl/BinaryOperator_impl.hpp"

#endif