#ifndef CIE_FEM_MATHS_UNIVARIATE_POLYNOMIAL_HPP
#define CIE_FEM_MATHS_UNIVARIATE_POLYNOMIAL_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/UnivariateScalarFunction.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::fem::maths {


template <concepts::NumericType NT>
class UnivariatePolynomial : public UnivariateScalarFunction<NT>
{
private:
    using base_type = UnivariateScalarFunction<NT>;

public:
    using coefficient_container = std::vector<NT>;

    using typename base_type::value_type;
    using typename base_type::derivative_ptr;

public:
    UnivariatePolynomial( coefficient_container&& r_coefficients );

    UnivariatePolynomial( const coefficient_container& r_coefficients );

    UnivariatePolynomial() = default;

    UnivariatePolynomial( UnivariatePolynomial<NT>&& r_rhs ) = default;

    UnivariatePolynomial( const UnivariatePolynomial<NT>& r_rhs ) = default;

    UnivariatePolynomial<NT>& operator=( const UnivariatePolynomial<NT>& r_rhs ) = default;

    virtual value_type operator()( NT argument ) const override;

    virtual derivative_ptr derivative() const override;

protected:
    coefficient_container _coefficients;
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/polynomial/impl/UnivariatePolynomial_impl.hpp"

#endif