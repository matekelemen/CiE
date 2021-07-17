#ifndef CIE_FEM_MATHS_SEPARABLE_MATRIX_FUNCTION_HPP
#define CIE_FEM_MATHS_SEPARABLE_MATRIX_FUNCTION_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/SeparableVectorFunction.hpp"
#include "FEM/packages/maths/inc/MatrixFunction.hpp"
#include "FEM/packages/maths/inc/NotImplementedFunction.hpp"

// --- STL Includes ---
#include <array>


namespace cie::fem::maths {


template <Size ValueDimension0, Size ValueDimension1, Size Dimension, concepts::NumericType NT>
class SeparableMatrixFunction : public MatrixFunction<ValueDimension0,ValueDimension1,Dimension,NT,NotImplementedFunction>
{
private:
    using base_type = MatrixFunction<ValueDimension0,ValueDimension1,Dimension,NT>;

public:
    using vector_function    = SeparableVectorFunction<ValueDimension1,Dimension,NT>;
    using function_ptr       = typename vector_function::SharedPointer;
    using function_container = std::array<function_ptr,ValueDimension0>;
    using function_list      = std::initializer_list<function_ptr>;

    using typename base_type::value_type;
    using typename base_type::argument_type;
    using typename base_type::derivative_ptr;

    CIE_DEFINE_CLASS_POINTERS( SeparableMatrixFunction )

public:
    SeparableMatrixFunction( function_container&& r_functions );

    SeparableMatrixFunction( const function_container& r_functions );

    SeparableMatrixFunction( function_list&& r_functions );

    SeparableMatrixFunction() = default;

    SeparableMatrixFunction( SeparableMatrixFunction<ValueDimension0,ValueDimension1,Dimension,NT>&& r_rhs ) = default;

    SeparableMatrixFunction( const SeparableMatrixFunction<ValueDimension0,ValueDimension1,Dimension,NT>& r_rhs ) = default;

    SeparableMatrixFunction<ValueDimension0,ValueDimension1,Dimension,NT>& operator=( const SeparableMatrixFunction<ValueDimension0,ValueDimension1,Dimension,NT>& r_rhs ) = default;

    value_type operator()( const argument_type& r_argument ) const override;

    derivative_ptr derivative() const override;

protected:
    function_container _functions;
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/impl/SeparableMatrixFunction_impl.hpp"

#endif