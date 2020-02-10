#ifndef GLVISUALIZATION_GL_CONTEXT_HPP
#define GLVISUALIZATION_GL_CONTEXT_HPP

#include <GLFW/glfw3.h>
#include "../inc/windowutilities.hpp"
#include "../../logging/inc/GLLogger.hpp"
#include <memory>
#include <string>

namespace cie {
namespace gl {


class GLContext;
using GLContextPtr  = std::shared_ptr<GLContext>;
using WindowPtr     = GLFWwindow*;


// GL context and window manager
class GLContext
{
public:
    GLContext(  uint8_t versionMajor = 4,
                uint8_t versionMinor = 5,
                uint8_t samples = 4);
    ~GLContext();

    WindowPtr openWindow(   size_t width                    = getPrimaryMonitorResolution().first,
                            size_t height                   = getPrimaryMonitorResolution().second,
                            const std::string& windowName   = "OpenGL",
                            GLFWmonitor* fullscreenMonitor  = glfwGetPrimaryMonitor(),
                            GLFWwindow* sharedWindow        = nullptr );
    void closeWindow();
    void makeContextCurrent();

    void startEventLoop( );

private:
    WindowPtr   _window;
    GLLogger    _logger;
};


}
}

#endif