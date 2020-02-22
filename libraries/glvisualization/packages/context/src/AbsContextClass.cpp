#include "../inc/AbsContextClass.hpp"

namespace cie {
namespace gl {


AbsContextClass::AbsContextClass(   GLContext& context,
                                    const std::string& className ) :
    _className( className ),
    _context( &context )
{
}


void AbsContextClass::log(  const std::string& message,
                            GLuint messageType )
{
    _context->log( _className + ":\t" + message, messageType );
}


void AbsContextClass::terminate( )
{
    _context->log( _className + ": Request termination" );
    _context->terminate();
}


}
}