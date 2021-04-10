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


template <Size ValueDimension0, Size ValueDimension1, Size Dimension, concepts::NumericType NT>
MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>
makeProduct( const ScalarFunctionPtr<Dimension,NT>& rp_lhs, const MatrixFunctionPtr<ValueDimension0,ValueDimension1,Dimension,NT>& rp_rhs );


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/operation/impl/Product_impl.hpp"

#endif