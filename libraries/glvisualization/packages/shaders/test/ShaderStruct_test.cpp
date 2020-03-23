// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "../inc/shaders.hpp"

// --- STD Includes ---
#include <string>
#include <iostream>

namespace cie {
namespace gl {


TEST_CASE( "ShaderStruct" )
{
    ShaderStruct vertexShader( "defaultColorVertexShader.glsl" );
    vertexShader.print();
}


}
}