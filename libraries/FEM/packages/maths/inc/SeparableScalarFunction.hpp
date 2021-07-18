#ifndef CIE_FEM_MATHS_SEPARABLE_SCALAR_FUNCTION_HPP
#define CIE_FEM_MATHS_SEPARABLE_SCALAR_FUNCTION_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/UnivariateScalarFunction.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::fem::maths {


// Forward declaration of the derivative type
template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
class SeparableVectorFunction;


/// A multivariate function representable as a product of univariate functions. 
template <Size Dimension, concepts::NumericType NT>
class SeparableScalarFunction : public ScalarFunction<Dimension,NT>
{
private:
    using base_type = ScalarFunction<Dimension,NT>;

public:
    using univariate_function  = UnivariateScalarFunction<NT>;
    using univariate_ptr       = typename univariate_function::SharedPointer;
    using univariate_container = std::array<univariate_ptr,Dimension>;
    using univariate_list      = std::initializer_list<univariate_ptr>;

    using typename base_type::value_type;
    using typename base_type::argument_type;
    using typename base_type::derivative_ptr;

    CIE_DEFINE_CLASS_POINTERS( SeparableScalarFunction )

public:
    SeparableScalarFunction( univariate_container&& r_univariates );

    SeparableScalarFunction( const univariate_container& r_univariates );

    SeparableScalarFunction( univariate_list&& r_univariates );

    SeparableScalarFunction();

    SeparableScalarFunction( SeparableScalarFunction<Dimension,NT>&& r_rhs ) = default;

    SeparableScalarFunction( const SeparableScalarFunction<Dimension,NT>& r_rhs ) = default;

    SeparableScalarFunction<Dimension,NT>& operator=( const SeparableScalarFunction<Dimension,NT>& r_rhs ) = default;

    value_type operator()( const argument_type& r_argument ) const override;

    derivative_ptr derivative() const override;

protected:
    univariate_container _univariates;
};


} // namespace cie::fem::maths

// Implementation
#include "FEM/packages/maths/impl/SeparableScalarFunction_impl.hpp"

// Dependence definition
#include "FEM/packages/maths/inc/SeparableVectorFunction.hpp"

#endif