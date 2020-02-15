#ifndef GLVISUALIZATION_DEFAULT_SHADERS
#define GLVISUALIZATION_DEFAULT_SHADERS

#include "../inc/shaders.hpp"

namespace cie {
namespace gl {

ShaderStruct defaultVertexShader(
R"glsl(

#version 450 core

in vec3 position;

void main()
{
    gl_Position = vec4( position, 1.0 );
}

)glsl",

{"position"}, {3},{0},{0},
{""} );


ShaderStruct defaultGeometryShader(
R"glsl(

#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3 ) out;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}

)glsl",

{}, {}, {}, {},
{});


ShaderStruct defaultFragmentShader(R"glsl(

#version 450 core

out vec4 color;

void main()
{
    color = vec4( 1.0, 1.0, 1.0, 1.0 );
}

)glsl",

{"color"}, {4}, {0}, {0},
{""});


}
}


#endif