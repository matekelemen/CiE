// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/GLFWScene.hpp"
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/context/inc/GLFWMonitor.hpp"


namespace cie::gl {


class TestScene : public GLFWScene
{
public:
    template <class ...Args>
    TestScene( Args&&... args ) : GLFWScene( std::forward<Args>(args)... ) {}

private:
    void update_impl() override {}
};


TEST_CASE( "GLFWScene", "[scene]" )
{
    CIE_TEST_CASE_INIT( "GLFWScene" )

    // Context
    std::pair<Size,Size> glVersion { 4, 5 };
    Size MSAASamples = 0;
    auto p_context = ContextPtr(
        new GLFWContext( glVersion.first,
                         glVersion.second,
                         MSAASamples,
                         TEST_OUTPUT_PATH + "/GLFWScene_test0.txt" )
    );

    // Window
    WindowPtr p_window;
    REQUIRE_NOTHROW( p_window = p_context->newWindow() );

    {
        CIE_TEST_CASE_INIT( "TestScene" )

        //auto p_scene = p_window->makeScene<TestScene>( "TestScene" );
    }
}


} // namespace cie::gl