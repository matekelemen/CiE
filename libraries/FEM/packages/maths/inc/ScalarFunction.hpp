#ifndef CIE_FEM_MATHS_SCALAR_FUNCTION_HPP
#define CIE_FEM_MATHS_SCALAR_FUNCTION_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/VectorFunction.hpp"
#include "FEM/packages/maths/inc/AbsFunction.hpp"
#include "FEM/packages/utilities/inc/kernel.hpp"


namespace cie::fem::maths {


template < Size Dimension,
           concepts::NumericType NT,
           class DerivativeType = VectorFunction<Dimension,Dimension,NT> >
struct ScalarFunction :
    public AbsFunction< NT,
                        typename Kernel<Dimension,NT>::point_type,
                        DerivativeType >,
    public Kernel<Dimension,NT>
{};


template < Size Dimension,
           concepts::NumericType NT >
using ScalarFunctionPtr = std::shared_ptr<ScalarFunction<Dimension,NT>>;


} // namespace cie::fem::maths


#endif