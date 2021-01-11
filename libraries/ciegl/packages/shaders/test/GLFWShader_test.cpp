// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shaders/inc/GLFWShader.hpp"
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/context/inc/GLFWMonitor.hpp"


namespace cie::gl {


CIE_TEST_CASE( "GLFWShader", "[shader]" )
{
    CIE_TEST_CASE_INIT( "GLFWShader" )

    // Context
    std::pair<Size,Size> glVersion { 4, 5 };
    Size MSAASamples = 0;
    auto p_context = GLFWContextSingleton::get(
        glVersion.first,
        glVersion.second,
        MSAASamples,
        TEST_OUTPUT_PATH / "GLFWShader_test.txt",
        false
    );

    // Window
    WindowPtr p_window;
    CIE_TEST_REQUIRE_NOTHROW( p_window = p_context->newWindow() );

    // Shader setup
    const std::filesystem::path shaderDir   = SOURCE_PATH / "libraries/ciegl/data/shaders/default";

    auto shaderPaths = [&shaderDir]( const std::filesystem::path& r_shaderName )
    {
        return std::pair<std::filesystem::path,std::filesystem::path>(
            (shaderDir / r_shaderName) += ".xml",
            (shaderDir / r_shaderName) += ".glsl"
        );
    };

    {
        CIE_TEST_CASE_INIT( "default vertex shader" )

        ShaderPtr p_shader;
        CIE_TEST_CHECK_NOTHROW( p_shader = makeVertexShader<GLFWVertexShader>( shaderPaths("vertexShader").first,
                                                                      shaderPaths("vertexShader").second ) );

        CIE_TEST_REQUIRE( !p_shader->attributes().empty() );
        CIE_TEST_CHECK( p_shader->attributes().size() == 1 );

        const auto& r_attribute = p_shader->attributes()[0];
        CIE_TEST_CHECK( r_attribute.name() == "position" );
        CIE_TEST_CHECK( r_attribute.size() == 3 );
        CIE_TEST_CHECK( r_attribute.stride() == 0 );
        CIE_TEST_CHECK( r_attribute.offset() == 0 );

        CIE_TEST_CHECK( p_shader->uniforms().empty() );
        CIE_TEST_CHECK( p_shader->textures().empty() );
        CIE_TEST_CHECK( p_shader->outputs().empty() );
    }

    {
        CIE_TEST_CASE_INIT( "default geometry shader" )

        ShaderPtr p_shader;
        CIE_TEST_CHECK_NOTHROW( p_shader = makeGeometryShader<GLFWGeometryShader>( shaderPaths("geometryShader").first,
                                                                          shaderPaths("geometryShader").second ) );
        
        CIE_TEST_CHECK( p_shader->attributes().empty() );
        CIE_TEST_CHECK( p_shader->uniforms().empty() );
        CIE_TEST_CHECK( p_shader->textures().empty() );
        CIE_TEST_CHECK( p_shader->outputs().empty() );
    }

    {
        CIE_TEST_CASE_INIT( "default fragment shader" )

        ShaderPtr p_shader;
        CIE_TEST_CHECK_NOTHROW( p_shader = makeFragmentShader<GLFWFragmentShader>( shaderPaths("fragmentShader").first,
                                                                          shaderPaths("fragmentShader").second ) );
    
        CIE_TEST_REQUIRE( !p_shader->outputs().empty() );
        CIE_TEST_CHECK( p_shader->outputs().size() == 1 );

        const auto& r_output = p_shader->outputs()[0];
        CIE_TEST_CHECK( r_output.name() == "color" );

        CIE_TEST_CHECK( p_shader->attributes().empty() );
        CIE_TEST_CHECK( p_shader->uniforms().empty() );
        CIE_TEST_CHECK( p_shader->textures().empty() );
    }
}


} // namespace cie::gl