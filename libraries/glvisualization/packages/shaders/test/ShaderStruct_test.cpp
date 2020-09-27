// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "glvisualization/packages/shaders/inc/shaders.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <string>
#include <iostream>
#include <memory>

namespace cie::gl {


TEST_CASE( "ShaderStruct", "[shaders]" )
{
    REQUIRE_NOTHROW( std::make_shared<ShaderStruct>( TEST_OUTPUT_PATH + "/defaultColorVertexShader.glsl" ) );
    //vertexShader.print();
}


} // namespace cie::gl