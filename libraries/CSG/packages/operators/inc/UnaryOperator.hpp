#ifndef CIE_CSG_UNARY_OPERATOR_HPP
#define CIE_CSG_UNARY_OPERATOR_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"


namespace cie::csg {


template < Size Dimension, 
           concepts::CopyConstructible ValueType = Bool,
           concepts::NumericType CoordinateType = Double >
class UnaryOperator : public CSGObject<Dimension,ValueType,CoordinateType>
{
public:
    using operand_type = CSGObject<Dimension,ValueType,CoordinateType>;
    using operand_ptr  = std::shared_ptr<operand_type>;

public:
    UnaryOperator() = default;
    UnaryOperator( operand_ptr p_rhs );
    UnaryOperator( const UnaryOperator<Dimension,ValueType,CoordinateType>& r_rhs ) = default;
    UnaryOperator<Dimension,ValueType,CoordinateType>& operator=( const UnaryOperator<Dimension,ValueType,CoordinateType>& r_rhs ) = default;

    void bindRhs( operand_ptr p_rhs );

    template <class CSGObjectType, class ...Args>
    void emplaceRhs( Args&&... args )
        requires concepts::DerivedFrom<CSGObjectType, typename UnaryOperator<Dimension, ValueType, CoordinateType>::operand_type>;

    operand_ptr& rhs();
    const operand_ptr& rhs() const;

    virtual ValueType at(const typename UnaryOperator<Dimension, ValueType, CoordinateType>::point_type& r_point) const override = 0;

protected:
    operand_ptr _p_rhs;
};


} // namespace cie::csg

#include "CSG/packages/operators/impl/UnaryOperator_impl.hpp"

#endif