#ifndef CIE_FEM_MATHS_OPERATOR_PRODUCT_IMPL_HPP
#define CIE_FEM_MATHS_OPERATOR_PRODUCT_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/operation/inc/Sum.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
ScalarFunctionPtr<Dimension,NT>
makeProduct( const ScalarFunctionPtr<Dimension,NT>& rp_lhs, const ScalarFunctionPtr<Dimension,NT>& rp_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using Operand = ScalarFunctionPtr<Dimension,NT>;
    using Argument = typename Operand::element_type::argument_type;
    using Result = typename Operand::element_type;
    using ResultPtr = std::shared_ptr<Result>;

    auto operation = [rp_lhs,rp_rhs]( const Argument& r_argument )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        return rp_lhs->operator()(r_argument) * rp_rhs->operator()(r_argument);
        CIE_END_EXCEPTION_TRACING
    };

    auto p_lhsDerivative = rp_lhs->derivative();
    auto p_rhsDerivative = rp_rhs->derivative();

    auto derivativeFactory = [ rp_lhs, rp_rhs,
                               p_lhsDerivative = std::move(p_lhsDerivative),
                               p_rhsDerivative = std::move(p_rhsDerivative)]
        ()
    {
        CIE_BEGIN_EXCEPTION_TRACING    
        return makeSum(
            makeProduct( rp_rhs, p_lhsDerivative ),
            makeProduct( rp_lhs, p_rhsDerivative )
        );
        CIE_END_EXCEPTION_TRACING
    };

    return ResultPtr(
        new GenericOperation<Result>(
            std::move( operation ),
            std::move( derivativeFactory )
        )
    );

    CIE_END_EXCEPTION_TRACING
}


template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
VectorFunctionPtr<ValueDimension,Dimension,NT>
makeProduct( const ScalarFunctionPtr<Dimension,NT>& rp_lhs, const VectorFunctionPtr<ValueDimension,Dimension,NT>& rp_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using ScalarOperand = ScalarFunction<Dimension,NT>;
    using Argument      = typename ScalarOperand::argument_type;
    using Result        = VectorFunction<ValueDimension,Dimension,NT>;

    auto operation = [rp_lhs, rp_rhs]( const Argument& r_argument )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        return rp_lhs->evaluate(r_argument) * rp_rhs->evaluate(r_argument);
        CIE_END_EXCEPTION_TRACING
    };

    auto p_lhsDerivative = rp_lhs->derivative();
    auto p_rhsDerivative = rp_rhs->derivative();

    auto derivativeFactory = [ rp_rhs, rp_lhs,
                               p_lhsDerivative=std::move(p_lhsDerivative),
                               p_rhsDerivative=std::move(p_rhsDerivative) ]
        ()
    {
        CIE_BEGIN_EXCEPTION_TRACING
        return makeSum(
            makeProduct( rp_rhs, p_lhsDerivative ),
            makeProduct( rp_lhs, p_rhsDerivative )
        );
        CIE_END_EXCEPTION_TRACING
    };

    return typename Result::SharedPointer(
        new GenericOperation<Result>(
            std::move( operation ),
            std::move( derivativeFactory )
        )
    );

    CIE_END_EXCEPTION_TRACING
}


template <Size LHSValueDimension, Size RHSValueDimension, Size Dimension, concepts::NumericType NT>
MatrixFunctionPtr<LHSValueDimension,RHSValueDimension,Dimension,NT>
makeProduct( const VectorFunctionPtr<LHSValueDimension,Dimension,NT>& rp_lhs,
             const VectorFunctionPtr<RHSValueDimension,Dimension,NT>& rp_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using Result   = MatrixFunction<LHSValueDimension,RHSValueDimension,Dimension,NT>;
    using Argument = typename Result::argument_type;

    auto operation = [rp_lhs, rp_rhs]( const Argument& r_argument )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        auto lhs = rp_lhs->evaluate( r_argument );
        auto rhs = rp_rhs->evaluate( r_argument );
        typename Result::value_type output;

        for ( Size i_row=0; i_row<LHSValueDimension; ++i_row )
            for ( Size i_column=0; i_column<RHSValueDimension; ++i_column )
                output( i_row, i_column ) = lhs[i_row] * rhs[i_column];

        return output;
        CIE_END_EXCEPTION_TRACING
    };

    return typename Result::SharedPointer(
        new GenericOperation<Result>(
            std::move( operation )
            // derivative not implemented
        )
    );

    CIE_END_EXCEPTION_TRACING
}


template <Size ValueDimension0, Size ValueDimension1, Size Dimension, concepts::NumericType NT>
MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>
makeProduct( const ScalarFunctionPtr<Dimension,NT>& rp_lhs, const MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>& rp_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using Result   = MatrixFunction<ValueDimension0,ValueDimension1,Dimension,NT>;
    using Argument = typename Result::argument_type;

    auto operation = [rp_lhs, rp_rhs]( const Argument& r_argument )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        return rp_rhs->evaluate( r_argument ) * rp_rhs->evaluate( r_argument );
        CIE_END_EXCEPTION_TRACING
    };

    return typename Result::SharedPointer(
        new GenericOperation<Result>(
            std::move( operation )
            // derivative not implemented
        )
    );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::AbsFunctionPtr LHSOperand>
NotImplementedFunctionPtr
makeProduct( const LHSOperand& r_lhs, const NotImplementedFunctionPtr& r_rhs )
{
    return NotImplementedFunctionPtr( new NotImplementedFunction );
}


template <concepts::AbsFunctionPtr RHSOperand>
NotImplementedFunctionPtr
makeProduct( const NotImplementedFunction& r_lhs, const RHSOperand& r_rhs )
{
    return NotImplementedFunctionPtr( new NotImplementedFunction );
}


} // namespace cie::fem::maths


#endif