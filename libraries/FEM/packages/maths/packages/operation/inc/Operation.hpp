#ifndef CIE_FEM_MATHS_OPERATION_HPP
#define CIE_FEM_MATHS_OPERATION_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/AbsFunction.hpp"


namespace cie::fem::maths {


template <concepts::AbsFunction ResultType>
using Operation = ResultType;


} // namespace cie::fem::maths


#endif