// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsContextClass.hpp"

namespace cie::gl {


AbsContextClass::AbsContextClass(   GLFWContext& context,
                                    const std::string& className ) :
    utils::Loggee( context, className ),
    _className( className ),
    _context( &context )
{
}


const GLFWContext* AbsContextClass::context() const
{
    return _context;
}


void AbsContextClass::checkGLErrors( const std::string& errorMessage )
{

    std::string message = errorMessage;
    if ( message.empty() )
        message = "GL Error";

    GLuint err = glGetError();
    if( err!=0 )
        logID(  message,
                err,
                LOG_TYPE_ERROR );
}


} // namespace cie::gl