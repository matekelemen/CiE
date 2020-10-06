// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shaders/inc/shaders.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <string>
#include <iostream>
#include <memory>

namespace cie::gl {


TEST_CASE( "ShaderStruct", "[shaders]" )
{
    CIE_TEST_CASE_INIT( "ShaderStruct" )

    REQUIRE_NOTHROW( std::make_shared<ShaderStruct>( TEST_OUTPUT_PATH + "/defaultColorVertexShader.glsl" ) );
    //vertexShader.print();
}


} // namespace cie::gl