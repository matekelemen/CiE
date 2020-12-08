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

class GLFWContext;
using DrawFunction          = std::function<bool()>;
using DrawFunctionFactory   = std::function<DrawFunction(GLFWContext&)>;


/* --- Define default functions and variables --- */

DrawFunction makeEmptyDrawFunction( GLFWContext& );


/* --- GLFWContext --- */

class GLFWContext final : public AbsContext
{
public:
    friend class GLFWContextSingleton;

public:
    ~GLFWContext();

    void focusWindow( WindowPtr p_window ) override;

private:
    /**
     * Initialize the GLFW context
     * 
     * @note this constructor is private and should not be called directly from anywhere
     * except from GLFWContextSingleton. See the overloads of GLFWContextSingleton::get
     */
    GLFWContext( Size versionMajor                    = 4,
                 Size versionMinor                    = 5,
                 Size MSAASamples                     = 0, 
                 const std::filesystem::path& r_logFileName = OUTPUT_PATH / "GLFWContext.log",
                 bool useConsole = false );

    /**
     * Check whether GLAD was initialized and initialize it if it wasn't
     */
    void initializeGLADIfNecessary();

    WindowPtr newWindow_impl( size_t width              = 800,
                              size_t height             = 600,
                              const std::string& r_name = "GLFW Window" ) override;

    void closeWindow_impl( WindowPtr p_window ) override;

private:
    bool  _isGLADInitialized;
    bool  _isCurrent;
};



/**
 * GLFWContext manager
 */
class GLFWContextSingleton
{
public:
    /**
     * Check whether the GLFWContext was initialized with the specified
     * parameters and create one if it was not.
     * 
     * @param versionMajor OpenGL standard version major specifier
     * (must match the version of the existing context)
     * @param versionMinor OpenGL standard version minor specifier
     * (must match the version of the existing context)
     * @param MSAASamples number of MSAASamples
     * (must match the version of the existing context)
     * @param r_logFilePath path to the log file
     * @param useConsole print log output to std::cout
     * 
     * @return a pointer to the GLFW context
     * @note throw an error if the requested parameters do not match the
     * parameters of the existing context
     */
    static ContextPtr get( Size versionMajor = 4,
                           Size versionMinor = 5,
                           Size MSAASamples  = 0,
                           const std::filesystem::path& r_logFilePath = OUTPUT_PATH / "GLFWContext.log",
                           bool useConsole = false );

    /**
     * Initialize the GLFW context or return the existing one,
     * and add a log file to its list of streams
     * 
     * @param r_logFilePath path to the log file
     * @return a pointer to the GLFW context
     */
    static ContextPtr get( const std::filesystem::path& r_logFilePath );

    /**
     * Initialize the GLFW context or return the existing one,
     * and add a log file to its list of streams
     * 
     * @param r_logFilePath path to the log file
     * @param useConsole print log output to std::cout
     * @return a pointer to the GLFW context
     */
    static ContextPtr get( const std::filesystem::path& r_logFilePath,
                           bool useConsole );

private:
    static ContextPtr _p_context;
};



} // namespace cie::gl

#endif