#ifndef GLVISUALIZATION_DEFAULT_SHADERS
#define GLVISUALIZATION_DEFAULT_SHADERS

#include "../inc/shaders.hpp"

namespace cie {
namespace gl {

ShaderStruct defaultColorVertexShader(
R"glsl(

#version 450 core

in vec3 position;
in vec3 vertexColor;
out vec3 vColor;

void main()
{
    vColor      = vertexColor;
    gl_Position = vec4( position, 1.0 );
}

)glsl",

{"position", "vertexColor"}, {3,3},{6,6},{0,3},
{""} );


ShaderStruct defaultColorGeometryShader(
R"glsl(

#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3 ) out;

in vec3 vColor[];
out vec3 fColor;

void main()
{
    fColor      = vColor[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    fColor      = vColor[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    fColor      = vColor[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}

)glsl",

{}, {}, {}, {},
{});


ShaderStruct defaultColorFragmentShader(R"glsl(

#version 450 core

in vec3 fColor;
out vec4 color;

void main()
{
    color = vec4( fColor, 1.0 );
}

)glsl",

{"color"}, {4}, {0}, {0},
{""});


}
}


#endif