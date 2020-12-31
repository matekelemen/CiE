// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsContextClass.hpp"

namespace cie::gl {


AbsContextClass::AbsContextClass(   GLFWContext& r_context,
                                    const std::string& r_name ) :
    utils::Loggee( r_context, r_name ),
    _r_context( r_context )
{
}


const GLFWContext& AbsContextClass::context() const
{
    return _r_context;
}


void AbsContextClass::checkGLErrors( const std::string& r_errorMessage )
{

    std::string message = r_errorMessage;
    if ( message.empty() )
        message = "GL Error";

    GLuint err = glGetError();
    if( err!=0 )
        logID(  message,
                err,
                LOG_TYPE_ERROR );
}


} // namespace cie::gl