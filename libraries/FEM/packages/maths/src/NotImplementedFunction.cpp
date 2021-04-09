// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/NotImplementedFunction.hpp"


namespace cie::fem::maths {


NotImplementedFunction::value_type NotImplementedFunction::operator()( const NotImplementedFunction::argument_type& r_argument ) const
{
    CIE_THROW(
        NotImplementedException,
        "Function type not implemented!"
    )
}


NotImplementedFunction::derivative_ptr NotImplementedFunction::derivative() const
{
    return NotImplementedFunction::derivative_ptr( new NotImplementedFunction );
}


} // namespace cie::fem::maths