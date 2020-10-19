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
                 const std::string& r_logFileName     = OUTPUT_PATH + "/ContextLogger.txt" );
    ~GLFWContext();

    WindowPtr newWindow( size_t width              = 800,
                         size_t height             = 600,
                         const std::string& r_name = "GLFW Window" ) override;

    void focusWindow( WindowPtr p_window ) override;

    void closeWindow( WindowPtr p_window ) override;

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