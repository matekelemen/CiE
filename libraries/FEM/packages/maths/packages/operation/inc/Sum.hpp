#ifndef CIE_FEM_MATHS_OPERATION_SUM_HPP
#define CIE_FEM_MATHS_OPERATION_SUM_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/MatrixFunction.hpp"
#include "FEM/packages/maths/inc/VectorFunction.hpp"
#include "FEM/packages/maths/inc/ScalarFunction.hpp"
#include "FEM/packages/maths/packages/operation/inc/GenericOperation.hpp"
#include "FEM/packages/maths/inc/NotImplementedFunction.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
ScalarFunctionPtr<Dimension,NT>
makeSum( const ScalarFunctionPtr<Dimension,NT>& rp_lhs,
         const ScalarFunctionPtr<Dimension,NT>& rp_rhs );


template <Size ValueDimension, Size Dimension, concepts::NumericType NT>
VectorFunctionPtr<ValueDimension,Dimension,NT>
makeSum( const VectorFunctionPtr<ValueDimension,Dimension,NT>& rp_lhs,
         const VectorFunctionPtr<ValueDimension,Dimension,NT>& rp_rhs );


template <Size ValueDimension0, Size ValueDimension1, Size Dimension, concepts::NumericType NT>
MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>
makeSum( const MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>& rp_lhs,
         const MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>& rp_rhs );


template <concepts::AbsFunctionPtr LHSOperand>
NotImplementedFunctionPtr
makeSum( const LHSOperand& r_lhs, const NotImplementedFunctionPtr& r_rhs );


template <concepts::AbsFunctionPtr RHSOperand>
NotImplementedFunctionPtr
makeSum( const NotImplementedFunctionPtr& r_lhs, const RHSOperand& r_rhs );


NotImplementedFunctionPtr
makeSum( const NotImplementedFunctionPtr& r_lhs, const NotImplementedFunctionPtr& r_rhs );


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/operation/impl/Sum_impl.hpp"

#endif