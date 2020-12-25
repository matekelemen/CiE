#ifndef CIE_CSG_COMPLEMENT_OPERATOR_IMPL_HPP
#define CIE_CSG_COMPLEMENT_OPERATOR_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
Complement<Dimension,CoordinateType>::Complement( typename Complement<Dimension,CoordinateType>::operand_ptr p_rhs ) :
    UnaryOperator<Dimension,Bool,CoordinateType>( p_rhs )
{
}


template <Size Dimension, concepts::NumericType CoordinateType>
inline Bool
Complement<Dimension,CoordinateType>::at( const typename Complement<Dimension,CoordinateType>::point_type& r_point ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( this->_p_rhs )

    return !this->_p_rhs->at( r_point );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg


#endif