#ifndef CIE_CSG_UNARY_OPERATOR_IMPL_HPP
#define CIE_CSG_UNARY_OPERATOR_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::csg {


template < Size Dimension,
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
UnaryOperator<Dimension,ValueType,CoordinateType>::UnaryOperator( typename UnaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr p_rhs ) :
    _p_rhs( p_rhs )
{
}


template < Size Dimension,
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
inline void
UnaryOperator<Dimension,ValueType,CoordinateType>::bindRhs( typename UnaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr p_rhs )
{
    this->_p_rhs = p_rhs;
}


template < Size Dimension,
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
template <class CSGObjectType, class ...Args>
inline void
UnaryOperator<Dimension,ValueType,CoordinateType>::emplaceRhs( Args&&... args )
    requires concepts::DerivedFrom<CSGObjectType,CSGObject<Dimension,ValueType,CoordinateType>>
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->_p_rhs = std::make_shared<CSGObjectType>( 
        std::forward<Args>(args)...
    );

    CIE_END_EXCEPTION_TRACING
}


template < Size Dimension,
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
inline typename UnaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr&
UnaryOperator<Dimension,ValueType,CoordinateType>::rhs()
{
    return this->_p_rhs;
}


template < Size Dimension,
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
inline const typename UnaryOperator<Dimension,ValueType,CoordinateType>::operand_ptr&
UnaryOperator<Dimension,ValueType,CoordinateType>::rhs() const
{
    return this->_p_rhs;
}


} // namespace cie::csg


#endif