#ifndef GLVISUALIZATION_GL_CONTEXT_HPP
#define GLVISUALIZATION_GL_CONTEXT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../inc/windowutilities.hpp"
#include "../../logging/inc/GLLogger.hpp"
#include "../inc/defaultCallback.hpp"
#include <functional>
#include <memory>
#include <string>

namespace cie {
namespace gl {


const GLuint CONTEXT_LOG_TYPE_REPORT    = 0;
const GLuint CONTEXT_LOG_TYPE_WARNING   = 1;
const GLuint CONTEXT_LOG_TYPE_ERROR     = 2;


class GLContext;
using WindowPtr             = GLFWwindow*;
using DrawFunction          = std::function<void()>;
using DrawFunctionFactory   = std::function<DrawFunction(GLContext&)>;



DrawFunction makeEmptyDrawFunction( GLContext& );



// GL context and window manager
class GLContext
{
public:
    GLContext(  uint8_t versionMajor                    = 4,
                uint8_t versionMinor                    = 5,
                uint8_t samples                         = 4, 
                const std::string& logFileName          = "DefaultGLLogger.txt" );
    ~GLContext();

    WindowPtr openWindow(   size_t width                    = getPrimaryMonitorResolution().first,
                            size_t height                   = getPrimaryMonitorResolution().second,
                            const std::string& windowName   = "OpenGL",
                            GLFWmonitor* fullscreenMonitor  = glfwGetPrimaryMonitor(),
                            GLFWwindow* sharedWindow        = nullptr );
    void closeWindow();
    void makeContextCurrent();
    void startEventLoop(    DrawFunctionFactory eventLoopGenerator = makeEmptyDrawFunction,
                            KeyCallbackFunction keyCallback             = callback_keyExit );

    void log(   const std::string& message,
                GLuint messageType = CONTEXT_LOG_TYPE_REPORT );
    void terminate( );

    WindowPtr window();
    const WindowPtr window() const;

private:
    WindowPtr           _window;
    GLLogger            _logger;
    DrawFunction        _drawFunction;
    
    static bool         _initialized;
    static bool         _active;
    static bool         _current;
};


}
}

#endif