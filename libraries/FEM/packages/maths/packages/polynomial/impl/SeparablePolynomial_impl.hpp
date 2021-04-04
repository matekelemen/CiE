#ifndef CIE_FEM_MATHS_SEPARABLE_POLYNOMIAL_IMPL_HPP
#define CIE_FEM_MATHS_SEPARABLE_POLYNOMIAL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
SeparablePolynomial<Dimension,NT>::SeparablePolynomial( typename SeparablePolynomial<Dimension,NT>::univariate_container&& r_polynomials ) :
    _polynomials( std::move(r_polynomials) )
{
}


template <Size Dimension, concepts::NumericType NT>
SeparablePolynomial<Dimension,NT>::SeparablePolynomial( const typename SeparablePolynomial<Dimension,NT>::univariate_container& r_polynomials ) :
    _polynomials( r_polynomials )
{
}


template <Size Dimension, concepts::NumericType NT>
SeparablePolynomial<Dimension,NT>::SeparablePolynomial() :
    _polynomials()
{
}


template <Size Dimension, concepts::NumericType NT>
inline typename SeparablePolynomial<Dimension,NT>::value_type
SeparablePolynomial<Dimension,NT>::operator()( const typename SeparablePolynomial<Dimension,NT>::argument_type& r_argument ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename SeparablePolynomial<Dimension,NT>::value_type value = 1;

    auto it_argument = r_argument.begin();

    for ( const auto& r_polynomial : this->_polynomials )
        value *= r_polynomial( *it_argument++ );

    return value;

    CIE_END_EXCEPTION_TRACING
}


template <Size Dimension, concepts::NumericType NT>
typename SeparablePolynomial<Dimension,NT>::derivative_type
SeparablePolynomial<Dimension,NT>::derivative() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename SeparablePolynomial<Dimension,NT>::derivative_type::polynomial_container derivatives;
    utils::resize( derivatives, Dimension );

    for ( Size dim=0; dim<Dimension; ++dim )
    {
        auto univariates = this->_polynomials;
        univariates[dim] = univariates[dim].derivative();
        derivatives[dim] = SeparablePolynomial<Dimension,NT>( univariates );
    }

    return typename SeparablePolynomial<Dimension,NT>::derivative_type( std::move(derivatives) );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif