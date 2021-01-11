// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsContext.hpp"

// --- STL Includes ---
#include <algorithm>

namespace cie::gl {


AbsContext::AbsContext( Size versionMajor,
                        Size versionMinor,
                        Size MSAASamples,
                        const std::filesystem::path& r_logFileName,
                        bool useConsole ) :
    utils::Logger( r_logFileName, useConsole ),
    _version( versionMajor, versionMinor ),
    _MSAASamples( MSAASamples ),
    _windowCounter( 0 )
{
}


AbsContext::~AbsContext()
{
    this->log( "Destroy context" );
}


const std::pair<Size,Size>&
AbsContext::version() const
{
    return _version;
}


const Size
AbsContext::MSAASamples() const
{
    return _MSAASamples;
}


WindowPtr AbsContext::newWindow( Size width,
                                 Size height,
                                 const std::string& r_name )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto scopedBlock = this->newBlock( "new window" );

    auto p_window = this->newWindow_impl( width,
                                          height,
                                          r_name );

    p_window->setID( this->_windowCounter++ );

    this->logs( "ID_", p_window->getID() );

    return p_window;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl