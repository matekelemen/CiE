#ifndef CIE_FEM_MATHS_SEPARABLE_VECTOR_POLYNOMIAL_HPP
#define CIE_FEM_MATHS_SEPARABLE_VECTOR_POLYNOMIAL_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/packages/polynomial/inc/SeparablePolynomial.hpp"
#include "FEM/packages/maths/inc/VectorFunction.hpp"

// --- STL Includes ---
#include <array>


namespace cie::fem::maths {


// Forward declaration of the derivative type
template <Size ValueDimension0, Size ValueDimension1, Size Dimension, concepts::NumericType NT>
class SeparableMatrixPolynomial;


/**
 * A vector of multivariate polynomials, each representable as a product of
 * univariate polynomials.
 */
template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
class SeparableVectorPolynomial : public VectorFunction<ValueDimension,Dimension,NT,SeparableMatrixPolynomial<ValueDimension,Dimension,Dimension,NT>>
{
public:
    using polynomial_container = std::array<SeparablePolynomial<Dimension,NT>,ValueDimension>;

private:
    using base_type = VectorFunction<ValueDimension,Dimension,NT,SeparableMatrixPolynomial<ValueDimension,Dimension,Dimension,NT>>;
    using typename base_type::value_type;
    using typename base_type::argument_type;
    using typename base_type::derivative_type;

public:
    SeparableVectorPolynomial( polynomial_container&& r_components );

    SeparableVectorPolynomial( const polynomial_container& r_components );

    SeparableVectorPolynomial() = default;

    SeparableVectorPolynomial( SeparableVectorPolynomial<ValueDimension,Dimension,NT>&& r_rhs ) = default;

    SeparableVectorPolynomial( const SeparableVectorPolynomial<ValueDimension,Dimension,NT>& r_rhs ) = default;

    SeparableVectorPolynomial<ValueDimension,Dimension,NT>& operator=( const SeparableVectorPolynomial<ValueDimension,Dimension,NT>& r_rhs ) = default;

    value_type operator()( const argument_type& r_argument ) const override;

    derivative_type derivative() const override;

protected:
    polynomial_container _components;
};


} // namespace cie::fem::maths

// Implementation
#include "FEM/packages/maths/packages/polynomial/impl/SeparableVectorPolynomial_impl.hpp"

// Dependence definitions
#include "FEM/packages/maths/packages/polynomial/inc/SeparableMatrixPolynomial.hpp"

#endif