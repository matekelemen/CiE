#ifndef CIE_FEM_MATHS_SEPARABLE_VECTOR_POLYNOMIAL_IMPL_HPP
#define CIE_FEM_MATHS_SEPARABLE_VECTOR_POLYNOMIAL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"


namespace cie::fem::maths {


template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
SeparableVectorPolynomial<ValueDimension,Dimension,NT>::SeparableVectorPolynomial( typename SeparableVectorPolynomial<ValueDimension,Dimension,NT>::polynomial_container&& r_components ) :
    _components( std::move(r_components) )
{
}


template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
SeparableVectorPolynomial<ValueDimension,Dimension,NT>::SeparableVectorPolynomial( const typename SeparableVectorPolynomial<ValueDimension,Dimension,NT>::polynomial_container& r_components ) :
    _components( r_components )
{
}


template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
inline typename SeparableVectorPolynomial<ValueDimension,Dimension,NT>::value_type
SeparableVectorPolynomial<ValueDimension,Dimension,NT>::operator()( const typename SeparableVectorPolynomial<ValueDimension,Dimension,NT>::argument_type& r_argument ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename SeparableVectorPolynomial<ValueDimension,Dimension,NT>::value_type value;
    utils::resize( value, ValueDimension );

    auto it_valueEnd = value.end();
    auto it_component = this->_components.begin();

    for ( auto it_value=value.begin(); it_value!=it_valueEnd; ++it_value,++it_component )
        *it_value = it_component->operator()( r_argument );

    return value;

    CIE_END_EXCEPTION_TRACING
}


template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
typename SeparableVectorPolynomial<ValueDimension,Dimension,NT>::derivative_type
SeparableVectorPolynomial<ValueDimension,Dimension,NT>::derivative() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename SeparableVectorPolynomial<ValueDimension,Dimension,NT>::derivative_type::polynomial_container derivatives;
    utils::resize( derivatives, Dimension );

    auto it_componentEnd = this->_components.end();
    auto it_derivative = derivatives.begin();

    for ( auto it_component=this->_components.begin(); it_component!=it_componentEnd; ++it_component,++it_derivative )
        *it_derivative = it_component->derivative();

    return typename SeparableVectorPolynomial<ValueDimension,Dimension,NT>::derivative_type( std::move(derivatives) );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif