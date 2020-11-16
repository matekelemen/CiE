#ifndef CIE_GL_ABS_CONTEXT_HPP
#define CIE_GL_ABS_CONTEXT_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"
#include "cieutils/packages/logging/inc/Logger.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsMonitor.hpp"
#include "ciegl/packages/context/inc/AbsWindow.hpp"

// --- STL Includes ---
#include <utility>
#include <memory>
#include <string>
#include <list>


namespace cie::gl {


/**
 * Interface for an OpenGL context.
*/
class AbsContext :
    public utils::Logger
{
public:
    using window_container = std::list<WindowPtr>;

public:

    AbsContext( Size versionMajor,
                Size versionMinor,
                Size MSAASamples,
                const std::string& r_logFileName,
                bool useConsole = false );

    ~AbsContext();

    /**
     * Get OpenGL version as {major,minor}
    */
    const std::pair<Size,Size>& version() const;

    /**
     * Get number of samples used for Multi Sample Anti-Aliasing.
    */
    const Size MSAASamples() const;

    const window_container& windows() const;

    WindowPtr newWindow( Size width                 = 800,
                         Size height                = 600,
                         const std::string& r_name  = "CiE" );

    /**
     * Make the specified window active.
    */
    virtual void focusWindow( WindowPtr p_window ) = 0;

    virtual void closeWindow( WindowPtr p_window );

    /**
     * Close every registered window
    */
    void closeAllWindows();

protected:
    virtual WindowPtr newWindow_impl( Size width,
                                      Size height,
                                      const std::string& r_name ) = 0;

    virtual void closeWindow_impl( WindowPtr p_window ) = 0;

private:

    /**
     * Append the internal set of windows.
     * Call after every call to newWindow.
    */
    void registerWindow( WindowPtr p_window );

    /**
     * Remove the specified window from the internal
     * set of windows.
    */
    void deregisterWindow( WindowPtr p_window );

protected:
    const std::pair<Size,Size>  _version;
    Size                        _MSAASamples;

private:
    window_container            _windows;
};


using ContextPtr        = std::shared_ptr<AbsContext>;
using ContextConstPtr   = std::shared_ptr<const AbsContext>;


} // namespace cie::gl

#endif