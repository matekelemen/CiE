#ifndef CIE_FEM_MATHS_OPERATION_SUM_IMPL_HPP
#define CIE_FEM_MATHS_OPERATION_SUM_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/warning.hpp"

// --- Linalg Includes ---
#include "linalg/packages/overloads/inc/vectoroperators.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
ScalarFunctionPtr<Dimension,NT>
makeSum( const ScalarFunctionPtr<Dimension,NT>& rp_lhs,
         const ScalarFunctionPtr<Dimension,NT>& rp_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using Operand = ScalarFunctionPtr<Dimension,NT>;
    using Argument = typename Operand::element_type::argument_type;
    using Result = typename Operand::element_type;
    using ResultPtr = std::shared_ptr<Result>;

    auto operation = [rp_lhs,rp_rhs]( const Argument& r_argument )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        return rp_lhs->evaluate(r_argument) + rp_rhs->evaluate(r_argument);
        CIE_END_EXCEPTION_TRACING
    };

    auto derivativeFactory = [rp_lhs,rp_rhs]()
    {
        CIE_BEGIN_EXCEPTION_TRACING
        return makeSum( rp_lhs->derivative(),  rp_rhs->derivative() );
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
makeSum( const VectorFunctionPtr<ValueDimension,Dimension,NT>& rp_lhs,
         const VectorFunctionPtr<ValueDimension,Dimension,NT>& rp_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using Operand = VectorFunctionPtr<ValueDimension,Dimension,NT>;
    using Argument = typename Operand::element_type::argument_type;
    using Result = typename Operand::element_type;
    using ResultPtr = std::shared_ptr<Result>;

    auto operation = [rp_lhs,rp_rhs]( const Argument& r_argument )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        return rp_lhs->evaluate(r_argument) + rp_rhs->evaluate(r_argument);
        CIE_END_EXCEPTION_TRACING
    };

    auto derivativeFactory = [rp_lhs,rp_rhs]()
    {
        CIE_BEGIN_EXCEPTION_TRACING
        return makeSum( rp_lhs->derivative(), rp_rhs->derivative() );
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


template <Size ValueDimension0, Size ValueDimension1, Size Dimension, concepts::NumericType NT>
MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>
makeSum( const MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>& rp_lhs,
         const MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>& rp_rhs )
{
    CIE_BEGIN_EXCEPTION_TRACING

    using Operand = MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>;
    using Argument = typename Operand::element_type::argument_type;
    using Result = typename Operand::element_type;
    using ResultPtr = std::shared_ptr<Result>;

    auto operation = [rp_lhs,rp_rhs]( const Argument& r_argument )
    {
        CIE_BEGIN_EXCEPTION_TRACING
        return rp_lhs->evaluate(r_argument) + rp_rhs->evaluate(r_argument);
        CIE_END_EXCEPTION_TRACING
    };

    auto derivativeFactory = [rp_lhs,rp_rhs]() -> typename Result::derivative_ptr
    {
        CIE_THROW(
            NotImplementedException,
            "Derivatives for MatrixFunction are not implemented"
        )
        return nullptr;
    };

    return ResultPtr(
        new GenericOperation<Result>(
            std::move( operation ),
            std::move( derivativeFactory )
        )
    );

    CIE_END_EXCEPTION_TRACING
}


template <concepts::AbsFunctionPtr LHSOperand>
NotImplementedFunctionPtr
makeSum( const LHSOperand& r_lhs, const NotImplementedFunctionPtr& r_rhs )
{
    CIE_WARNING( "creating a sum with an invalid right operand" )
    return NotImplementedFunctionPtr( new NotImplementedFunction );
}


template <concepts::AbsFunctionPtr RHSOperand>
NotImplementedFunctionPtr
makeSum( const NotImplementedFunctionPtr& r_lhs, const RHSOperand& r_rhs )
{
    CIE_WARNING( "creating a sum with an invalid left operand" )
    return NotImplementedFunctionPtr( new NotImplementedFunction );
}


} // namespace cie::fem::maths


#endif