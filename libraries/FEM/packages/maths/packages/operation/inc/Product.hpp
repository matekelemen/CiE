#ifndef CIE_FEM_MATHS_OPERATOR_PRODUCT_HPP
#define CIE_FEM_MATHS_OPERATOR_PRODUCT_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/MatrixFunction.hpp"
#include "FEM/packages/maths/inc/VectorFunction.hpp"
#include "FEM/packages/maths/inc/ScalarFunction.hpp"
#include "FEM/packages/maths/packages/operation/inc/GenericOperation.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
ScalarFunctionPtr<Dimension,NT>
makeProduct( const ScalarFunctionPtr<Dimension,NT>& rp_lhs, const ScalarFunctionPtr<Dimension,NT>& rp_rhs );


template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
VectorFunctionPtr<ValueDimension,Dimension,NT>
makeProduct( const ScalarFunctionPtr<Dimension,NT>& rp_lhs, const VectorFunctionPtr<ValueDimension,Dimension,NT>& rp_rhs );


/// Creates a tensor product
template <Size LHSValueDimension, Size RHSValueDimension, Size Dimension, concepts::NumericType NT>
MatrixFunctionPtr<LHSValueDimension,RHSValueDimension,Dimension,NT>
makeProduct( const VectorFunctionPtr<LHSValueDimension,Dimension,NT>& rp_lhs,
             const VectorFunctionPtr<RHSValueDimension,Dimension,NT>& rp_rhs );


template <Size ValueDimension0, Size ValueDimension1, Size Dimension, concepts::NumericType NT>
MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>
makeProduct( const ScalarFunctionPtr<Dimension,NT>& rp_lhs, const MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>& rp_rhs );


template <concepts::AbsFunctionPtr LHSOperand>
NotImplementedFunctionPtr
makeProduct( const LHSOperand& r_lhs, const NotImplementedFunctionPtr& r_rhs );


template <concepts::AbsFunctionPtr RHSOperand>
NotImplementedFunctionPtr
makeProduct( const NotImplementedFunction& r_lhs, const RHSOperand& r_rhs );


NotImplementedFunctionPtr
makeProduct( const NotImplementedFunctionPtr& r_lhs, const NotImplementedFunctionPtr& r_rhs );


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/operation/impl/Product_impl.hpp"

#endif