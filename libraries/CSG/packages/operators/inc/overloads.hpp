#ifndef CIE_CSG_OVERLOADS_HPP
#define CIE_CSG_OVERLOADS_HPP

// --- Internal Includes ---
#include "CSG/packages/operators/inc/Complement.hpp"
#include "CSG/packages/operators/inc/Intersection.hpp"
#include "CSG/packages/operators/inc/Union.hpp"
#include "CSG/packages/operators/inc/Subtraction.hpp"


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
IntersectionPtr<Dimension,CoordinateType> operator*( CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs,
                                                     CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs );


template <Size Dimension, concepts::NumericType CoordinateType>
UnionPtr<Dimension,CoordinateType> operator+( CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs,
                                              CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs );


template <Size Dimension, concepts::NumericType CoordinateType>
SubtractionPtr<Dimension,CoordinateType> operator-( CSGObjectPtr<Dimension,Bool,CoordinateType> p_lhs,
                                                    CSGObjectPtr<Dimension,Bool,CoordinateType> p_rhs );


} // namespace cie::csg

#include "CSG/packages/operators/impl/overloads_impl.hpp"

#endif