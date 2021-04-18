// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/context/inc/GLFWMonitor.hpp"
#include "cmake_variables.hpp"


namespace cie::gl {


CIE_TEST_CASE( "GLFWContext", "[context]" )
{
    CIE_TEST_CASE_INIT( "GLFWContext" )

    // Window parameters
    std::pair<Size,Size> windowSize   { 800, 600 };
    const std::string windowName    = "GLFW testwindow";

    {
        CIE_TEST_CASE_INIT( "Context creation" )

        ContextPtr p_context;

        CIE_TEST_REQUIRE_NOTHROW(
            p_context = gl::GLFWContextSingleton::get()
        );
    }

    {
        CIE_TEST_CASE_INIT( "Window creation" )

        auto p_context = gl::GLFWContextSingleton::get();

        WindowPtr p_window;
        CIE_TEST_REQUIRE_NOTHROW(
            p_window = p_context->newWindow(
                windowSize.first,
                windowSize.second,
                windowName
            )
        );

        CIE_TEST_CHECK_NOTHROW( p_context->focusWindow(p_window) );
    }

}


} // namespace cie::gl