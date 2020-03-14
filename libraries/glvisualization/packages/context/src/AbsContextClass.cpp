// --- Internal Includes ---
#include "../inc/AbsContextClass.hpp"

namespace cie {
namespace gl {


AbsContextClass::AbsContextClass(   GLContext& context,
                                    const std::string& className ) :
    utils::Loggee( context, className ),
    _className( className ),
    _context( &context )
{
}


void AbsContextClass::terminate( )
{
    log( "Request termination" );
    _context->terminate();
}


}
}