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

    //auto derivativeFactory = [rp_lhs,rp_rhs,p_lhsDerivative,p_rhsDerivative]()
    //{
    //    CIE_BEGIN_EXCEPTION_TRACING
    //    auto p_lhsDerivative = rp_lhs->derivative();
    //    auto p_rhsDerivative = rp_rhs->derivative();
    //    return makeSum(
    //        makeProduct( rp_rhs, p_lhsDerivative ),
    //        makeProduct( rp_lhs, p_rhsDerivative )
    //    );
    //    CIE_END_EXCEPTION_TRACING
    //};

    return ResultPtr(
        new GenericOperation<Result>(
            std::move( operation )
            //,std::move( derivativeFactory )
        )
    );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif