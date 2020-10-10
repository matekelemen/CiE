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


/* --- Type aliases --- */

class GLFWContext;
using WindowPtr             = GLFWwindow*;
using MonitorPtr            = GLFWmonitor*;

using DrawFunction          = std::function<bool()>;
using DrawFunctionFactory   = std::function<DrawFunction(GLFWContext&)>;


/* --- Define default functions and variables --- */

DrawFunction makeEmptyDrawFunction( GLFWContext& );


/* --- Define smart pointers for GLFW objects with custom destructors --- */

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
    GLFWWindowPtr() :
        std::shared_ptr<GLFWwindow>( nullptr ) {}
    GLFWWindowPtr( GLFWwindow* p_window ) : 
        std::shared_ptr<GLFWwindow>( p_window, GLFWWindowDestructorFunctor() ) {}
};

struct GLFWMonitorPtr : public std::shared_ptr<GLFWmonitor>
{
    GLFWMonitorPtr() :
        std::shared_ptr<GLFWmonitor>( nullptr ) {}
    GLFWMonitorPtr( GLFWmonitor* p_monitor ) : 
        std::shared_ptr<GLFWmonitor>( p_monitor, GLFWMonitorDestructorFunctor() ) {}
};

} // namespace detail

using GLFWWindowPtr     = detail::GLFWWindowPtr;
using GLFWMonitorPtr    = detail::GLFWMonitorPtr;


/* --- GLFWContext --- */

class GLFWContext final : public AbsContext<GLFWwindow,GLFWmonitor,GLFWWindowPtr,GLFWMonitorPtr>
{
public:
    GLFWContext( Size versionMajor                    = 4,
                 Size versionMinor                    = 5,
                 Size MSAASamples                     = 0, 
                 const std::string& r_logFileName     = OUTPUT_PATH + "/ContextLogger.txt" );
    ~GLFWContext();

    typename GLFWContext::window_ptr newWindow( size_t width                              = 800,
                                                size_t height                             = 600,
                                                const std::string& r_name                 = "GLFW Window",
                                                typename GLFWContext::monitor_ptr p_monitor = nullptr ) override;

    void focusWindow( typename GLFWContext::window_ptr p_window ) override;

    void closeWindow( typename GLFWContext::window_ptr p_window ) override;

    void startEventLoop(    DrawFunctionFactory eventLoopGenerator  = makeEmptyDrawFunction,
                            KeyCallbackFunction keyCallback         = callback_keyExit,
                            CursorCallbackFunction cursorCallback   = defaultCursorCallbackFunction,
                            MouseCallbackFunction mouseCallback     = defaultMouseCallbackFunction );

    WindowPtr window();
    const WindowPtr window() const;

private:
    void initializeGLADIfNecessary();

private:
    DrawFunction                    _drawFunction;
    
    static bool                     _isGLFWInitialized;
    static bool                     _active;
    static bool                     _isGLADInitialized;
};


} // namespace cie::gl

#endif