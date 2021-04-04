#ifndef CIE_FEM_MATHS_UNIVARIATE_POLYNOMIAL_HPP
#define CIE_FEM_MATHS_UNIVARIATE_POLYNOMIAL_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/ScalarFunction.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::fem::maths {


template <concepts::NumericType NT>
class UnivariatePolynomial : public ScalarFunction<1,NT,UnivariatePolynomial<NT>>
{
public:
    using coefficient_container = std::vector<NT>;

public:
    UnivariatePolynomial( coefficient_container&& r_coefficients );

    UnivariatePolynomial( const coefficient_container& r_coefficients );

    UnivariatePolynomial() = default;

    UnivariatePolynomial( UnivariatePolynomial<NT>&& r_rhs ) = default;

    UnivariatePolynomial( const UnivariatePolynomial<NT>& r_rhs ) = default;

    UnivariatePolynomial<NT>& operator=( const UnivariatePolynomial<NT>& r_rhs ) = default;

    virtual typename UnivariatePolynomial::value_type operator()( const typename UnivariatePolynomial<NT>::argument_type& r_argument ) const override;

    typename UnivariatePolynomial::value_type operator()( NT argument ) const;

    virtual typename UnivariatePolynomial<NT>::derivative_type derivative() const override;

protected:
    coefficient_container _coefficients;
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/polynomial/impl/UnivariatePolynomial_impl.hpp"

#endif