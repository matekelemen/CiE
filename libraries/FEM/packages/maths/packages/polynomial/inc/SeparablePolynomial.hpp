#ifndef CIE_FEM_MATHS_SEPARABLE_POLYNOMIAL_HPP
#define CIE_FEM_MATHS_SEPARABLE_POLYNOMIAL_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/packages/polynomial/inc/UnivariatePolynomial.hpp"
#include "FEM/packages/maths/inc/ScalarFunction.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::fem::maths {


// Forward declaration of the derivative type
template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
class SeparableVectorPolynomial;


/**
 * A multivariate polynomial representable as a product of univariate polynomials. 
 */
template <Size Dimension, concepts::NumericType NT>
class SeparablePolynomial : public ScalarFunction<Dimension,NT,SeparableVectorPolynomial<Dimension,Dimension,NT>>
{
public:
    using univariate_container = std::array<UnivariatePolynomial<NT>,Dimension>;

private:
    using base_type = ScalarFunction<Dimension,NT,SeparableVectorPolynomial<Dimension,Dimension,NT>>;
    using typename base_type::value_type;
    using typename base_type::argument_type;
    using typename base_type::derivative_type;

public:
    SeparablePolynomial( univariate_container&& r_polynomials );

    SeparablePolynomial( const univariate_container& r_polynomials );

    SeparablePolynomial();

    SeparablePolynomial( SeparablePolynomial<Dimension,NT>&& r_rhs ) = default;

    SeparablePolynomial( const SeparablePolynomial<Dimension,NT>& r_rhs ) = default;

    SeparablePolynomial<Dimension,NT>& operator=( const SeparablePolynomial<Dimension,NT>& r_rhs ) = default;

    value_type operator()( const argument_type& r_argument ) const override;

    derivative_type derivative() const override;

protected:
    univariate_container _polynomials;
};


} // namespace cie::fem::maths

// Implementation
#include "FEM/packages/maths/packages/polynomial/impl/SeparablePolynomial_impl.hpp"

// Dependence definition
#include "FEM/packages/maths/packages/polynomial/inc/SeparableVectorPolynomial.hpp"

#endif