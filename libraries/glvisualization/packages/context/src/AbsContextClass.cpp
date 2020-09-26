// --- Internal Includes ---
#include "glvisualization/packages/context/inc/AbsContextClass.hpp"

namespace cie::gl {


AbsContextClass::AbsContextClass(   GLContext& context,
                                    const std::string& className ) :
    utils::Loggee( context, className ),
    _className( className ),
    _context( &context )
{
}


void AbsContextClass::terminate( )
{
    log( "Request context termination" );
    _context->terminate();
}


const GLContext* AbsContextClass::context() const
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