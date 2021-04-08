#ifndef CIE_FEM_MATHS_UNIVARIATE_LAGRANGE_BASIS_POLYNOMIAL_HPP
#define CIE_FEM_MATHS_UNIVARIATE_LAGRANGE_BASIS_POLYNOMIAL_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/polynomial/inc/UnivariatePolynomial.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::fem::maths {


template <concepts::NumericType NT>
class UnivariateLagrangeBasisPolynomial : public UnivariatePolynomial<NT>
{
public:
    template <concepts::NumericContainer NodeContainer>
    UnivariateLagrangeBasisPolynomial( NodeContainer&& r_nodes, Size basisIndex );

    UnivariateLagrangeBasisPolynomial() = default;

    UnivariateLagrangeBasisPolynomial( UnivariateLagrangeBasisPolynomial<NT>&& r_rhs ) = default;

    UnivariateLagrangeBasisPolynomial( const UnivariateLagrangeBasisPolynomial<NT>& r_rhs ) = default;

    UnivariateLagrangeBasisPolynomial<NT>& operator=( const UnivariateLagrangeBasisPolynomial<NT>& r_rhs ) = default;
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/polynomial/impl/UnivariateLagrangeBasisPolynomial_impl.hpp"

#endif