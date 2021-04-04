#ifndef CIE_FEM_MATHS_SEPARABLE_MATRIX_POLYNOMIAL_HPP
#define CIE_FEM_MATHS_SEPARABLE_MATRIX_POLYNOMIAL_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/packages/polynomial/inc/SeparableVectorPolynomial.hpp"
#include "FEM/packages/maths/inc/MatrixFunction.hpp"
#include "FEM/packages/maths/inc/NotImplementedFunction.hpp"

// --- STL Includes ---
#include <array>


namespace cie::fem::maths {


template <Size ValueDimension0, Size ValueDimension1, Size Dimension, concepts::NumericType NT>
class SeparableMatrixPolynomial : public MatrixFunction<ValueDimension0,ValueDimension1,Dimension,NT,NotImplementedFunction>
{
public:
    using polynomial_container = std::array<SeparableVectorPolynomial<ValueDimension1,Dimension,NT>,ValueDimension0>;

private:
    using base_type = MatrixFunction<ValueDimension0,ValueDimension1,Dimension,NT>;
    using typename base_type::value_type;
    using typename base_type::argument_type;
    using typename base_type::derivative_type;

public:
    SeparableMatrixPolynomial( polynomial_container&& r_vectorPolynomials );

    SeparableMatrixPolynomial( const polynomial_container& r_vectorPolynomials );

    SeparableMatrixPolynomial() = default;

    SeparableMatrixPolynomial( SeparableMatrixPolynomial<ValueDimension0,ValueDimension1,Dimension,NT>&& r_rhs ) = default;

    SeparableMatrixPolynomial( const SeparableMatrixPolynomial<ValueDimension0,ValueDimension1,Dimension,NT>& r_rhs ) = default;

    SeparableMatrixPolynomial<ValueDimension0,ValueDimension1,Dimension,NT>& operator=( const SeparableMatrixPolynomial<ValueDimension0,ValueDimension1,Dimension,NT>& r_rhs ) = default;

    value_type operator()( const argument_type& r_argument ) const override;

    derivative_type derivative() const override;

protected:
    polynomial_container _vectorPolynomials;
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/polynomial/impl/SeparableMatrixPolynomial_impl.hpp"

#endif