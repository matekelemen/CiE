#ifndef GLVISUALIZATION_GL_CONTEXT_HPP
#define GLVISUALIZATION_GL_CONTEXT_HPP

#include <GLFW/glfw3.h>
#include "../inc/windowutilities.hpp"
#include "../../logging/inc/GLLogger.hpp"
#include "../inc/callbacks_keys.hpp"
#include <functional>
#include <memory>
#include <string>

namespace cie {
namespace gl {


class GLContext;
using WindowPtr                 = GLFWwindow*;
using EventLoopFunction         = std::function<void()>;
using EventLoopFunctionFactory  = std::function<EventLoopFunction(GLContext&)>;



EventLoopFunction makeEmptyEventLoopFunction( GLContext& );



// GL context and window manager
class GLContext
{
public:
    GLContext(  uint8_t versionMajor                    = 4,
                uint8_t versionMinor                    = 5,
                uint8_t samples                         = 4, 
                EventLoopFunctionFactory loopFactory    = makeEmptyEventLoopFunction,
                const std::string& logFileName          = "DefaultGLLogger.txt" );
    ~GLContext();

    WindowPtr openWindow(   size_t width                    = getPrimaryMonitorResolution().first,
                            size_t height                   = getPrimaryMonitorResolution().second,
                            const std::string& windowName   = "OpenGL",
                            GLFWmonitor* fullscreenMonitor  = glfwGetPrimaryMonitor(),
                            GLFWwindow* sharedWindow        = nullptr );
    void closeWindow();
    void makeContextCurrent();
    void startEventLoop( KeyCallbackFunction keyCallback    = callback_keyExit );

    GLLogger& logger();
    WindowPtr window();
    const WindowPtr window() const;

private:
    WindowPtr           _window;
    GLLogger            _logger;
    EventLoopFunction   _eventLoop;
};


}
}

#endif