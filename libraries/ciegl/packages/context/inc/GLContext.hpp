#ifndef CIEGL_GL_CONTEXT_HPP
#define CIEGL_GL_CONTEXT_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Utility Includes ---
#include <cieutils/logging.hpp>
#include <cieutils/observer.hpp>

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsContext.hpp"
#include "ciegl/packages/context/inc/windowutilities.hpp"
#include "ciegl/packages/context/inc/defaultCallback.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <functional>
#include <memory>
#include <string>

namespace cie::gl {


// Define type aliases
class GLContext;
using WindowPtr             = GLFWwindow*;
using MonitorPtr            = GLFWmonitor*;

using DrawFunction          = std::function<bool()>;
using DrawFunctionFactory   = std::function<DrawFunction(GLContext&)>;

// Define default functions and variables
DrawFunction makeEmptyDrawFunction( GLContext& );


namespace detail {

struct GLFWWindowDestructorFunctor
{
    void operator()( GLFWwindow* p_window ) { glfwDestroyWindow(p_window); }
};

struct GLFWMonitorDestructorFunctor
{
    void operator()( GLFWmonitor* p_monitor ) { /*TODO*/ }
};

struct GLFWWindowPtr : public std::shared_ptr<GLFWwindow>
{
    GLFWWindowPtr( GLFWwindow* p_window ) : 
        std::shared_ptr<GLFWwindow>( p_window, GLFWWindowDestructorFunctor() ) {}
};

struct GLFWMonitorPtr : public std::shared_ptr<GLFWmonitor>
{
    GLFWMonitorPtr( GLFWmonitor* p_monitor ) : 
        std::shared_ptr<GLFWmonitor>( p_monitor, GLFWMonitorDestructorFunctor() ) {}
};

} // namespace detail

using GLFWWindowPtr     = detail::GLFWWindowPtr;
using GLFWMonitorPtr    = detail::GLFWMonitorPtr;


class GLContext final : public AbsContext<GLFWwindow,GLFWmonitor,GLFWWindowPtr,GLFWMonitorPtr>
{
public:
    GLContext(  Size versionMajor                    = 4,
                Size versionMinor                    = 5,
                Size MSAASamples                     = 0, 
                const std::string& r_logFileName     = OUTPUT_PATH + "/ContextLogger.txt" );
    ~GLContext();

    typename GLContext::window_ptr newWindow( size_t width                              = 800,
                                              size_t height                             = 600,
                                              const std::string& r_name                 = "GLFW Window",
                                              typename GLContext::monitor_ptr p_monitor = nullptr ) override;
    void closeWindow( typename GLContext::window_ptr p_window ) override;
    void makeContextCurrent();
    void startEventLoop(    DrawFunctionFactory eventLoopGenerator  = makeEmptyDrawFunction,
                            KeyCallbackFunction keyCallback         = callback_keyExit,
                            CursorCallbackFunction cursorCallback   = defaultCursorCallbackFunction,
                            MouseCallbackFunction mouseCallback     = defaultMouseCallbackFunction );

    void terminate( );

    WindowPtr window();
    const WindowPtr window() const;

private:
    DrawFunction                    _drawFunction;
    
    static bool                     _initialized;
    static bool                     _active;
    static bool                     _current;
};


} // namespace cie::gl

#endif