// --- Utility Includes ---
#include "cieutils/packages/macros/inc/warning.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/operation/inc/Sum.hpp"


namespace cie::fem::maths {


NotImplementedFunctionPtr
makeSum( const NotImplementedFunctionPtr& r_lhs, const NotImplementedFunctionPtr& r_rhs )
{
    CIE_WARNING( "creating a sum with invalid operands" )
    return NotImplementedFunctionPtr( new NotImplementedFunction );
}


} // namespace cie::fem::maths