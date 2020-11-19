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
                         TEST_OUTPUT_PATH / "GLFWShader_test.txt" )
    );

    // Window
    WindowPtr p_window;
    REQUIRE_NOTHROW( p_window = p_context->newWindow() );

    // Shader setup
    const std::string shaderDir   = SOURCE_PATH / "libraries/ciegl/data/shaders/default";

    auto shaderPaths = [&shaderDir]( const std::string& r_shaderName )
    {
        return std::pair<std::string,std::string>(
            shaderDir / std::filesystem::path(r_shaderName + ".xml"),
            shaderDir / std::filesystem::path(r_shaderName + ".glsl")
        );
    };

    {
        CIE_TEST_CASE_INIT( "default vertex shader" )

        ShaderPtr p_shader;
        CHECK_NOTHROW( p_shader = makeVertexShader<GLFWVertexShader>( shaderPaths("vertexShader").first,
                                                                      shaderPaths("vertexShader").second ) );

        REQUIRE( !p_shader->attributes().empty() );
        CHECK( p_shader->attributes().size() == 1 );

        const auto& r_attribute = p_shader->attributes()[0];
        CHECK( r_attribute.name() == "position" );
        CHECK( r_attribute.size() == 3 );
        CHECK( r_attribute.stride() == 0 );
        CHECK( r_attribute.offset() == 0 );

        CHECK( p_shader->uniforms().empty() );
        CHECK( p_shader->textures().empty() );
        CHECK( p_shader->outputs().empty() );
    }

    {
        CIE_TEST_CASE_INIT( "default geometry shader" )

        ShaderPtr p_shader;
        CHECK_NOTHROW( p_shader = makeGeometryShader<GLFWGeometryShader>( shaderPaths("geometryShader").first,
                                                                          shaderPaths("geometryShader").second ) );
        
        CHECK( p_shader->attributes().empty() );
        CHECK( p_shader->uniforms().empty() );
        CHECK( p_shader->textures().empty() );
        CHECK( p_shader->outputs().empty() );
    }

    {
        CIE_TEST_CASE_INIT( "default fragment shader" )

        ShaderPtr p_shader;
        CHECK_NOTHROW( p_shader = makeFragmentShader<GLFWFragmentShader>( shaderPaths("fragmentShader").first,
                                                                          shaderPaths("fragmentShader").second ) );
    
        REQUIRE( !p_shader->outputs().empty() );
        CHECK( p_shader->outputs().size() == 1 );

        const auto& r_output = p_shader->outputs()[0];
        CHECK( r_output.name() == "color" );

        CHECK( p_shader->attributes().empty() );
        CHECK( p_shader->uniforms().empty() );
        CHECK( p_shader->textures().empty() );
    }
}


} // namespace cie::gl