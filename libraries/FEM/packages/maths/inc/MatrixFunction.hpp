#ifndef CIE_FEM_MATHS_MATRIX_FUNCTION_HPP
#define CIE_FEM_MATHS_MATRIX_FUNCTION_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/NotImplementedFunction.hpp"
#include "FEM/packages/maths/inc/AbsFunction.hpp"
#include "FEM/packages/utilities/inc/kernel.hpp"


namespace cie::fem::maths {


template < Size ValueDimension0,
           Size ValueDimension1,
           Size Dimension,
           concepts::NumericType NT,
           class DerivativeType = NotImplementedFunction >
using MatrixFunction = AbsFunction< typename Kernel<Dimension,NT>::dense::matrix_type<ValueDimension0,ValueDimension1>,
                                    typename Kernel<Dimension,NT>::point_type,
                                    DerivativeType >;


} // namespace cie::fem::maths

#endif