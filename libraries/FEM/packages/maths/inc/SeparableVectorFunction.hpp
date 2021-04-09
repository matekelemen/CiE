#ifndef CIE_FEM_MATHS_SEPARABLE_VECTOR_FUNCTION_HPP
#define CIE_FEM_MATHS_SEPARABLE_VECTOR_FUNCTION_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/SeparableScalarFunction.hpp"
#include "FEM/packages/maths/inc/VectorFunction.hpp"

// --- STL Includes ---
#include <array>


namespace cie::fem::maths {


// Forward declaration of the derivative type
template <Size ValueDimension0, Size ValueDimension1, Size Dimension, concepts::NumericType NT>
class SeparableMatrixFunction;


/**
 * A vector of multivariate polynomials, each representable as a product of
 * univariate polynomials.
 */
template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
class SeparableVectorFunction : public VectorFunction<ValueDimension,Dimension,NT,SeparableMatrixFunction<ValueDimension,Dimension,Dimension,NT>>
{
private:
    using base_type = VectorFunction<ValueDimension,Dimension,NT,SeparableMatrixFunction<ValueDimension,Dimension,Dimension,NT>>;

public:
    using scalar_function = SeparableScalarFunction<Dimension,NT>;
    using function_ptr = std::shared_ptr<scalar_function>;
    using function_container = std::array<function_ptr,ValueDimension>;

    using typename base_type::value_type;
    using typename base_type::argument_type;
    using typename base_type::derivative_ptr;

public:
    SeparableVectorFunction( function_container&& r_components );

    SeparableVectorFunction( const function_container& r_components );

    SeparableVectorFunction() = default;

    SeparableVectorFunction( SeparableVectorFunction<ValueDimension,Dimension,NT>&& r_rhs ) = default;

    SeparableVectorFunction( const SeparableVectorFunction<ValueDimension,Dimension,NT>& r_rhs ) = default;

    SeparableVectorFunction<ValueDimension,Dimension,NT>& operator=( const SeparableVectorFunction<ValueDimension,Dimension,NT>& r_rhs ) = default;

    value_type operator()( const argument_type& r_argument ) const override;

    derivative_ptr derivative() const override;

protected:
    function_container _components;
};


} // namespace cie::fem::maths

// Implementation
#include "FEM/packages/maths/impl/SeparableVectorFunction_impl.hpp"

// Dependence definitions
#include "FEM/packages/maths/inc/SeparableMatrixFunction.hpp"

#endif