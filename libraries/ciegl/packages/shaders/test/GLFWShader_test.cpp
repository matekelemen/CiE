// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/context/inc/GLFWMonitor.hpp"


namespace cie::gl {


TEST_CASE( "GLFWShader", "[shader]" )
{
    CIE_TEST_CASE_INIT( "GLFWShader" )

    // Context
    std::pair<Size,Size> glVersion { 4, 5 };
    Size MSAASamples = 0;
    auto p_context = ContextPtr(
        new GLFWContext( glVersion.first,
                         glVersion.second,
                         MSAASamples,
                         TEST_OUTPUT_PATH + "/GLFWShader_test.txt" )
    );

    // Window
    WindowPtr p_window;
    REQUIRE_NOTHROW( p_window = p_context->newWindow() );

    // Shader setup
    const std::string shaderDir   = SOURCE_PATH + "/libraries/ciegl/data/shaders";
    const std::string shaderName  = "defaultVertexShader";

    const std::string configPath  = shaderDir + "/" + shaderName + ".xml";
    const std::string codePath    = shaderDir + "/" + shaderName + ".glsl";

    {
        CIE_TEST_CASE_INIT( "default vertex shader" )

        ShaderPtr p_shader;
        CHECK_NOTHROW( p_shader = makeVertexShader<GLFWVertexShader>( configPath, codePath ) );
    }
}


} // namespace cie::gl