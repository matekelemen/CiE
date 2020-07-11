// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/shaders.hpp"

// --- STL Includes ---
#include <string>
#include <iostream>
#include <memory>

namespace cie::gl {


TEST_CASE( "ShaderStruct" )
{
    REQUIRE_NOTHROW( std::make_shared<ShaderStruct>( "defaultColorVertexShader.glsl" ) );
    //vertexShader.print();
}


}