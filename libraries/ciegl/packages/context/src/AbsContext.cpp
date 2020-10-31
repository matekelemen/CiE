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
                        const std::string& r_logFileName,
                        bool useConsole ) :
    utils::Logger( r_logFileName, useConsole ),
    utils::AbsSubject(),
    _version( versionMajor, versionMinor ),
    _MSAASamples( MSAASamples ),
    _windows()
{
}


AbsContext::~AbsContext()
{
    if ( !this->_windows.empty() )
        this->warn( "The derived context should have closed all windows, but "
                    + std::to_string( this->_windows.size() )
                    + " are still open" );

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
    this->registerWindow( p_window );

    this->logs( "ID_", p_window->getID() );

    return p_window;

    CIE_END_EXCEPTION_TRACING
}


const typename AbsContext::window_container&
AbsContext::windows() const
{
    return _windows;
}


void AbsContext::closeWindow( WindowPtr p_window )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto scopedBlock = this->newBlock( "close window" );

    this->closeWindow_impl( p_window );

    // Max use count:
    //  - one pointer with which this function was called
    //  - one pointer constructed as the argument
    //  - one registered in the internal container
    if ( p_window.use_count() > 3 )
        this->error<Exception>( "Cannot force window to be destroyed while it's being used elsewhere | ID_" + std::to_string(p_window->getID()));

    this->deregisterWindow( p_window );
    this->logs( "ID_", p_window->getID() );

    //p_window.reset();

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