#ifndef CIEGL_GL_GLFW_CONTEXT_HPP
#define CIEGL_GL_GLFW_CONTEXT_HPP

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
#include <filesystem>

namespace cie::gl {


/**
 * Used as a static in the constructor of GLFWContext. Initializes GLFW
 * exactly once, when the first GLFWContext is created, and terminates
 * GLFW exactly once at program termination.
 * 
 * As a sideeffect, only one version of GLFW context can be created and
 * attempts to create a different one will result in an exception.
 */
namespace detail {
struct GLFWRAII
{
    explicit GLFWRAII( Size versionMajor,
                       Size versionMinor,
                       Size MSAASamples,
                       utils::Logger& r_logger );
    ~GLFWRAII();

    const Size _versionMajor;
    const Size _versionMinor;
    const Size _MSAASamples;
};
}


class GLFWContext;
using DrawFunction          = std::function<bool()>;
using DrawFunctionFactory   = std::function<DrawFunction(GLFWContext&)>;


/* --- Define default functions and variables --- */

DrawFunction makeEmptyDrawFunction( GLFWContext& );


/* --- GLFWContext --- */

class GLFWContext final : public AbsContext
{
public:
    GLFWContext( Size versionMajor                    = 4,
                 Size versionMinor                    = 5,
                 Size MSAASamples                     = 0, 
                 const std::filesystem::path& r_logFileName     = OUTPUT_PATH / "ContextLogger.txt",
                 bool useConsole = false );

    ~GLFWContext();

    void focusWindow( WindowPtr p_window ) override;

    void startEventLoop( DrawFunctionFactory eventLoopGenerator = makeEmptyDrawFunction );

    WindowPtr window();
    const WindowPtr window() const;

private:
    void initializeGLADIfNecessary();

    WindowPtr newWindow_impl( size_t width              = 800,
                              size_t height             = 600,
                              const std::string& r_name = "GLFW Window" ) override;

    void closeWindow_impl( WindowPtr p_window ) override;

private:
    DrawFunction                    _drawFunction;

    static bool                     _isGLADInitialized;
    static bool                     _isCurrent;
};


} // namespace cie::gl

#endif