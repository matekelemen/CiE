#ifndef GLVISUALIZATION_GL_CONTEXT_HPP
#define GLVISUALIZATION_GL_CONTEXT_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Utility Includes ---
#include <cieutils/logging.hpp>
#include <cieutils/observer.hpp>

// --- Internal Includes ---
#include "glvisualization/packages/context/inc/windowutilities.hpp"
#include "glvisualization/packages/context/inc/defaultCallback.hpp"
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




class GLContext : public utils::Logger, public utils::AbsSubject
{
public:
    GLContext(  uint8_t versionMajor                    = 4,
                uint8_t versionMinor                    = 5,
                uint8_t samples                         = 0, 
                const std::string& logFileName          = OUTPUT_PATH + "/ContextLogger.txt" );
    ~GLContext();

    WindowPtr openWindow(   size_t width                    = 800,
                            size_t height                   = 600,
                            const std::string& windowName   = "OpenGL",
                            GLFWmonitor* fullscreenMonitor  = nullptr,
                            WindowPtr sharedWindow          = nullptr );
    void closeWindow();
    void makeContextCurrent();
    void startEventLoop(    DrawFunctionFactory eventLoopGenerator  = makeEmptyDrawFunction,
                            KeyCallbackFunction keyCallback         = callback_keyExit,
                            CursorCallbackFunction cursorCallback   = defaultCursorCallbackFunction,
                            MouseCallbackFunction mouseCallback     = defaultMouseCallbackFunction );

    void terminate( );

    WindowPtr window();
    const WindowPtr window() const;

private:
    WindowPtr                       _window;
    DrawFunction                    _drawFunction;
    
    static bool                     _initialized;
    static bool                     _active;
    static bool                     _current;
};


} // namespace cie::gl

#endif