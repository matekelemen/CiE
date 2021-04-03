#ifndef CIE_FEM_MATHS_POLYNOMIAL_1D_IMPL_HPP
#define CIE_FEM_MATHS_POLYNOMIAL_1D_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"


namespace cie::fem::maths {


template <concepts::NumericType NT>
Polynomial1D<NT>::Polynomial1D( typename Polynomial1D<NT>::coefficient_container&& r_coefficients ) :
    _coefficients( std::move(r_coefficients) )
{
}


template <concepts::NumericType NT>
Polynomial1D<NT>::Polynomial1D( const typename Polynomial1D<NT>::coefficient_container& r_coefficients ) :
    _coefficients( r_coefficients )
{
}


template <concepts::NumericType NT>
Polynomial1D<NT>::Polynomial1D() :
    Polynomial1D<NT>( {0} )
{
}


template <concepts::NumericType NT>
Polynomial1D<NT>::Polynomial1D( const Polynomial1D<NT>& r_rhs ) :
    _coefficients( r_rhs._coefficients )
{
}


template <concepts::NumericType NT>
typename Polynomial1D<NT>::value_type
Polynomial1D<NT>::operator()( const typename Polynomial1D<NT>::argument_type& r_argument ) const
{
    return this->operator()( r_argument[0] );
}


template <concepts::NumericType NT>
typename Polynomial1D<NT>::value_type
Polynomial1D<NT>::operator()( NT argument ) const
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
typename Polynomial1D<NT>::derivative_type
Polynomial1D<NT>::derivative() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename Polynomial1D<NT>::coefficient_container derivativeCoefficients;

    Size polynomialOrder = this->_coefficients.size();

    if ( 1 < polynomialOrder )
    {
        utils::resize( derivativeCoefficients, polynomialOrder - 1 );

        const NT* p_coefficient = &this->_coefficients[1];
        NT* p_derivativeCoefficient = &derivativeCoefficients[0];

        for ( Size power=1; power<polynomialOrder; ++power )
            *p_derivativeCoefficient++ = power * (*p_coefficient++);
    }

    return Polynomial1D<NT>( derivativeCoefficients );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif