#ifndef CIE_GL_ABS_CONTEXT_HPP
#define CIE_GL_ABS_CONTEXT_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <utility>
#include <memory>
#include <string>
#include <list>


namespace cie::gl {


/**
 * Interface for an OpenGL context.
*/
template <  class WindowType,
            class MonitorType >
class AbsContext
{
public:
    using window_type           = WindowType;
    using window_ptr            = std::shared_ptr<window_type>;
    using window_container      = std::list<window_ptr>;

    using monitor_type          = MonitorType;
    using monitor_ptr           = std::shared_ptr<monitor_type>;

public:

    AbsGLContext( Size versionMajor,
                  Size versionMinor,
                  Size MSAASamples,
                  const std::string& r_logFileName );

    virtual ~AbsContext() = 0;

    /**
     * Get OpenGL version as {major,minor}
    */
    const std::pair<Size,Size>& version() const;

    /**
     * Get number of samples used for Multi Sample Anti-Aliasing.
    */
   const Size MSAASamples() const;

    /**
     * Get the set of all registered windows.
    */
    const window_container& windows() const;

    /**
     * Open a new window. Must call registerWindow before returning.
    */
    virtual window_ptr newWindow(   Size width,
                                    Size height,
                                    const std::string& r_name,
                                    monitor_ptr p_monitor ) = 0;

    /**
     * Close the specified window. Must call deregisterWindow before
     * returning.
    */
    virtual void closeWindow( window_ptr window );

    /**
     * Close every registered window
    */
    void closeAllWindows();

    /**
     * Cleanup
    */
    virtual void terminate() = 0;

protected:

    /**
     * Append the internal set of windows.
     * Call after every call to newWindow.
    */
    void registerWindow( window_ptr p_window );

    /**
     * Remove the specified window from the internal
     * set of windows.
    */
    void deregisterWindow( window_ptr p_window );

protected:
    const std::pair<Size,Size>  _version;
    Size                        _MSAASamples;

private:
    window_container            _windows;
};


} // namespace cie::gl

#include "ciegl/packages/context/impl/AbsContext_impl.hpp"

#endif