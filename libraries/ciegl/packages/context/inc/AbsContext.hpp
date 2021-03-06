#ifndef CIE_GL_ABS_CONTEXT_HPP
#define CIE_GL_ABS_CONTEXT_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/types.hpp"
#include "cieutils/packages/logging/inc/Logger.hpp"
#include "cieutils/packages/logging/inc/LoggerSingleton.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsMonitor.hpp"
#include "ciegl/packages/context/inc/AbsWindow.hpp"

// --- STL Includes ---
#include <utility>
#include <memory>
#include <string>
#include <list>
#include <filesystem>


namespace cie::gl {


/**
 * Interface for an OpenGL context.
*/
class AbsContext
{
public:
    using window_container = std::list<WindowPtr>;

public:

    AbsContext( Size versionMajor,
                Size versionMinor,
                Size MSAASamples,
                utils::LoggerPtr p_logger = utils::LoggerSingleton::getPtr() );

    ~AbsContext();

    /// Get OpenGL version as {major,minor}
    const std::pair<Size,Size>& version() const;

    /// Get number of samples used for Multi Sample Anti-Aliasing.
    const Size MSAASamples() const;

    WindowPtr newWindow( Size width                 = 800,
                         Size height                = 600,
                         const std::string& r_name  = "CiE" );

    /// Make the specified window active.
    virtual void focusWindow( WindowPtr p_window ) = 0;

    utils::Logger& logger();

protected:
    virtual WindowPtr newWindow_impl( Size width,
                                      Size height,
                                      const std::string& r_name ) = 0;

    virtual void closeWindow_impl( WindowPtr p_window ) = 0;

protected:
    const std::pair<Size,Size>  _version;
    Size                        _MSAASamples;

    Size                        _windowCounter;
    utils::LoggerPtr            _p_logger;
};


using ContextPtr        = std::shared_ptr<AbsContext>;
using ContextConstPtr   = std::shared_ptr<const AbsContext>;


} // namespace cie::gl

#endif