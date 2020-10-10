#ifndef CIE_GL_ABS_CONTEXT_IMPL_HPP
#define CIE_GL_ABS_CONTEXT_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>

namespace cie::gl {


template < class WindowType,
           class MonitorType,
           class WindowPtr,
           class MonitorPtr >
AbsContext<WindowType,MonitorType,WindowPtr,MonitorPtr>::AbsContext( Size versionMajor,
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


template < class WindowType,
           class MonitorType,
           class WindowPtr,
           class MonitorPtr >
AbsContext<WindowType,MonitorType,WindowPtr,MonitorPtr>::~AbsContext()
{
    closeAllWindows();
}



template < class WindowType,
           class MonitorType,
           class WindowPtr,
           class MonitorPtr >
inline const std::pair<Size,Size>&
AbsContext<WindowType,MonitorType,WindowPtr,MonitorPtr>::version() const
{
    return _version;
}


template < class WindowType,
           class MonitorType,
           class WindowPtr,
           class MonitorPtr >
inline const Size
AbsContext<WindowType,MonitorType,WindowPtr,MonitorPtr>::MSAASamples() const
{
    return _MSAASamples;
}


template < class WindowType,
           class MonitorType,
           class WindowPtr,
           class MonitorPtr >
inline const typename AbsContext<WindowType,MonitorType,WindowPtr,MonitorPtr>::window_container&
AbsContext<WindowType,MonitorType,WindowPtr,MonitorPtr>::windows() const
{
    return _windows;
}


template < class WindowType,
           class MonitorType,
           class WindowPtr,
           class MonitorPtr >
inline void
AbsContext<WindowType,MonitorType,WindowPtr,MonitorPtr>::closeAllWindows()
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto windows = _windows;

    for ( auto& rp_window : windows )
        this->closeWindow( rp_window );

    CIE_END_EXCEPTION_TRACING
}


template < class WindowType,
           class MonitorType,
           class WindowPtr,
           class MonitorPtr >
inline void
AbsContext<WindowType,MonitorType,WindowPtr,MonitorPtr>::registerWindow( WindowPtr p_window )
{
    CIE_BEGIN_EXCEPTION_TRACING

    _windows.push_back( p_window );

    CIE_END_EXCEPTION_TRACING
}


template < class WindowType,
           class MonitorType,
           class WindowPtr,
           class MonitorPtr >
inline void
AbsContext<WindowType,MonitorType,WindowPtr,MonitorPtr>::deregisterWindow( WindowPtr p_window )
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


#endif