#ifndef CIE_CSG_INTERSECTION_OPERATOR_HPP
#define CIE_CSG_INTERSECTION_OPERATOR_HPP

// --- Internal Includes ---
#include "CSG/packages/operators/inc/BinaryOperator.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
class Intersection : public BinaryOperator<Dimension,Bool,CoordinateType>
{
public:
    Intersection() = default;
    Intersection( typename Intersection<Dimension,CoordinateType>::operand_ptr p_lhs,
           typename Intersection<Dimension,CoordinateType>::operand_ptr p_rhs );
    Intersection( const Intersection<Dimension,CoordinateType>& r_rhs ) = default;
    Intersection<Dimension,CoordinateType>& operator=( const Intersection<Dimension,CoordinateType>& r_rhs ) = default;

    virtual Bool at( const typename Intersection<Dimension,CoordinateType>::point_type& r_point ) const override;
};


template <Size Dimension, concepts::NumericType CoordinateType>
using IntersectionPtr = std::shared_ptr<Intersection<Dimension,CoordinateType>>;


} // namespace cie::csg

#include "CSG/packages/operators/impl/Intersection_impl.hpp"

#endif