#ifndef CIE_FEM_MATHS_VECTOR_FUNCTION_HPP
#define CIE_FEM_MATHS_VECTOR_FUNCTION_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/MatrixFunction.hpp"
#include "FEM/packages/maths/inc/AbsFunction.hpp"
#include "FEM/packages/utilities/inc/kernel.hpp"


namespace cie::fem::maths {


template < Size ValueDimension,
           Size Dimension,
           concepts::NumericType NT,
           class DerivativeType = MatrixFunction<ValueDimension,Dimension,Dimension,NT> >
using VectorFunction = AbsFunction< typename Kernel<ValueDimension,NT>::point_type,
                                    typename Kernel<Dimension,NT>::point_type,
                                    DerivativeType >;


} // namespace cie::fem::maths


#endif