// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/buffer/inc/GLFWBufferManager.hpp"


namespace cie::gl {


CIE_TEST_CASE( "GLFWBufferManager", "[buffer]" )
{
    CIE_TEST_CASE_INIT( "GLFWBufferManager" )

    // Context
    std::pair<Size,Size> glVersion { 4, 5 };
    Size MSAASamples = 0;
    auto p_context = GLFWContextSingleton::get(
        glVersion.first,
        glVersion.second,
        MSAASamples,
        TEST_OUTPUT_PATH / "GLFWBufferManager_test.txt",
        false
    );

    // Window
    WindowPtr p_window;
    CIE_TEST_REQUIRE_NOTHROW( p_window = p_context->newWindow() );

    // Buffers
    BufferManagerPtr p_bufferManager;
    VertexBufferPtr p_vertexBuffer;
    ElementBufferPtr p_elementBuffer;

    {
        CIE_TEST_CASE_INIT( "create buffer manager" )
        CIE_TEST_REQUIRE_NOTHROW( p_bufferManager.reset( new GLFWBufferManager(*p_context) ));
    }

    {
        CIE_TEST_CASE_INIT( "generate buffers" )

        CIE_TEST_CHECK_NOTHROW( p_vertexBuffer = p_bufferManager->makeVertexBuffer() );
        CIE_TEST_CHECK_NOTHROW( p_elementBuffer = p_bufferManager->makeElementBuffer() );
    }

    {
        CIE_TEST_CASE_INIT( "bind buffers" )

        CIE_TEST_CHECK_NOTHROW( p_bufferManager->bindVertexBuffer(p_vertexBuffer) );
        CIE_TEST_CHECK_NOTHROW( p_bufferManager->bindElementBuffer(p_elementBuffer) );
    }
}


} // namespace cie::gl