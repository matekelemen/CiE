// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/context/inc/GLFWMonitor.hpp"


namespace cie::gl {


TEST_CASE( "GLFWContext", "[context]" )
{
    CIE_TEST_CASE_INIT( "GLFWContext" )

    // Window parameters
    std::pair<Size,Size> windowSize   { 800, 600 };
    const std::string windowName    = "GLFW testwindow";

    {
        CIE_TEST_CASE_INIT( "Context creation" )

        ContextPtr p_context;

        REQUIRE_NOTHROW(
            p_context = gl::GLFWContextSingleton::get(
                TEST_OUTPUT_PATH / "GLFWContext_test0.log",
                false
            )
        );
    }

    {
        CIE_TEST_CASE_INIT( "Window creation" )

        auto p_context = gl::GLFWContextSingleton::get(
            TEST_OUTPUT_PATH / "GLFWContext_test1.log",
            false
        );

        WindowPtr p_window;
        REQUIRE_NOTHROW(
            p_window = p_context->newWindow(
                windowSize.first,
                windowSize.second,
                windowName
            )
        );

        CHECK_NOTHROW( p_context->focusWindow(p_window) );
        REQUIRE_NOTHROW( p_context->closeWindow(p_window) );
    }

}


} // namespace cie::gl