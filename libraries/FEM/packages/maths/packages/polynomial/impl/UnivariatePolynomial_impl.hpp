#ifndef CIE_FEM_MATHS_UNIVARIATE_POLYNOMIAL_IMPL_HPP
#define CIE_FEM_MATHS_UNIVARIATE_POLYNOMIAL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"


namespace cie::fem::maths {


template <concepts::NumericType NT>
UnivariatePolynomial<NT>::UnivariatePolynomial( typename UnivariatePolynomial<NT>::coefficient_container&& r_coefficients ) :
    _coefficients( std::move(r_coefficients) )
{
}


template <concepts::NumericType NT>
UnivariatePolynomial<NT>::UnivariatePolynomial( const typename UnivariatePolynomial<NT>::coefficient_container& r_coefficients ) :
    _coefficients( r_coefficients )
{
}


template <concepts::NumericType NT>
typename UnivariatePolynomial<NT>::value_type
UnivariatePolynomial<NT>::operator()( NT argument ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    NT value = 0;

    if ( !this->_coefficients.empty() ) [[likely]]
    {
        NT position = argument;
        NT x = 1;
        

        for ( NT coefficient : this->_coefficients )
        {
            value += coefficient * x;
            x *= position;
        }
    }

    return value;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::NumericType NT>
typename UnivariatePolynomial<NT>::derivative_ptr
UnivariatePolynomial<NT>::derivative() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename UnivariatePolynomial<NT>::coefficient_container derivativeCoefficients;

    Size polynomialOrder = this->_coefficients.size();

    if ( 1 < polynomialOrder )
    {
        utils::resize( derivativeCoefficients, polynomialOrder - 1 );

        const NT* p_coefficient = &this->_coefficients[1];
        NT* p_derivativeCoefficient = &derivativeCoefficients[0];

        for ( Size power=1; power<polynomialOrder; ++power )
            *p_derivativeCoefficient++ = power * (*p_coefficient++);
    }

    return typename UnivariatePolynomial<NT>::derivative_ptr(
        new UnivariatePolynomial<NT>( std::move(derivativeCoefficients) )
    );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif