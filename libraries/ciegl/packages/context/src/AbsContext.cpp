// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>

namespace cie::gl {


AbsContext::AbsContext( Size versionMajor,
                        Size versionMinor,
                        Size MSAASamples,
                        const std::string& r_logFileName ) :
    utils::Logger( r_logFileName ),
    utils::AbsSubject(),
    _version( versionMajor, versionMinor ),
    _MSAASamples( MSAASamples ),
    _windows()
{
}


AbsContext::~AbsContext()
{
    closeAllWindows();
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


const typename AbsContext::window_container&
AbsContext::windows() const
{
    return _windows;
}


void AbsContext::closeWindw( WindowPtr p_window )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( p_window.use_count() != 2 )
        CIE_THROW( Exception, "Cannot force window to be destroyed while it's being used elsewhere" )

    deregisterWindow( p_window );
    p_window.reset();

    CIE_END_EXCEPTION_TRACING
}


void
AbsContext::closeAllWindows()
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto windows = _windows;

    for ( auto& rp_window : windows )
        this->closeWindow( rp_window );

    CIE_END_EXCEPTION_TRACING
}


void
AbsContext::registerWindow( WindowPtr p_window )
{
    CIE_BEGIN_EXCEPTION_TRACING

    _windows.push_back( p_window );

    CIE_END_EXCEPTION_TRACING
}


void
AbsContext::deregisterWindow( WindowPtr p_window )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto itp_window = std::find( _windows.begin(),
                                 _windows.end(),
                                 p_window );

    if ( itp_window != _windows.end() )
        _windows.erase( itp_window );
    else
    {
        CIE_THROW( OutOfRangeException, "Window not found in container" )
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl