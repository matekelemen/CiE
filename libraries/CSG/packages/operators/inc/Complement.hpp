#ifndef CIE_CSG_COMPLEMENT_OPERATOR_HPP
#define CIE_CSG_COMPLEMENT_OPERATOR_HPP

// --- Internal Includes ---
#include "CSG/packages/operators/inc/UnaryOperator.hpp"


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
class Complement : public UnaryOperator<Dimension,Bool,CoordinateType>
{
public:
    Complement() = default;
    Complement( typename Complement<Dimension,CoordinateType>::operand_ptr p_rhs );
    Complement( const Complement<Dimension,CoordinateType>& r_rhs ) = default;
    Complement<Dimension,CoordinateType>& operator=( const Complement<Dimension,CoordinateType>& r_rhs ) = default;

    virtual Bool at( const typename Complement<Dimension,CoordinateType>::point_type& r_point ) const override;
};


} // namespace cie::csg

#include "CSG/packages/operators/impl/Complement_impl.hpp"

#endif