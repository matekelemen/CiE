#ifndef GLVISUALIZATION_SPACETREE_SHADERS_IMPL_HPP
#define GLVISUALIZATION_SPACETREE_SHADERS_IMPL_HPP

namespace cie {
namespace gl {


const ShaderStruct SpaceTreeVertexShader(
R"glsl(

#version 450 core

// Vertex layout:
//  poition      : x, y, z
//  cell size    : size


in vec3     position;
in float    cellSize;
out float   vCellSize;

uniform mat4 transformation;

void main()
{
    vCellSize   = cellSize;
    gl_Position = transformation * vec4( position, 1.0 );
}

)glsl",

{"position", "cellSize"}, {3,1},{4,4},{0,3},
{"transformation"}, {} );


const ShaderStruct SpaceTreeGeometryShader(
R"glsl(

#version 450 core

layout(points) in;
layout(line_strip, max_vertices=6 ) out;

in float vCellSize[];

void main()
{
    gl_Position = gl_in[0].gl_Position - vCellSize[0]/2.0f * vec4(1.0f, 0.0f, 0.0f, 1.0f);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vCellSize[0]/2.0f * vec4(1.0f, 0.0f, 0.0f, 1.0f);
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[0].gl_Position - vCellSize[0]/2.0f * vec4(0.0f, 1.0f, 0.0f, 1.0f);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vCellSize[0]/2.0f * vec4(0.0f, 1.0f, 0.0f, 1.0f);
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[0].gl_Position - vCellSize[0]/2.0f * vec4(0.0f, 0.0f, 1.0f, 1.0f);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vCellSize[0]/2.0f * vec4(0.0f, 0.0f, 1.0f, 1.0f);
    EmitVertex();
    EndPrimitive();
}

)glsl",

{}, {}, {}, {},
{}, {} );


const ShaderStruct SpaceTreeFragmentShader(R"glsl(

#version 450 core

out vec4 color;

void main()
{
    color = vec4( 1.0, 1.0, 1.0, 1.0 );
}

)glsl",

{"color"}, {4}, {0}, {0},
{}, {} );


const ShaderStruct SpaceTreeColorVertexShader(
R"glsl(

#version 450 core

in vec3 position;
in vec3 vertexColor;
out vec3 vColor;

uniform mat4 transformation;

void main()
{
    vColor      = vertexColor;
    gl_Position = transformation * vec4( position, 1.0 );
}

)glsl",

{"position", "vertexColor"}, {3,3},{6,6},{0,3},
{"transformation"}, {} );


const ShaderStruct SpaceTreeColorGeometryShader(
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
{}, {} );


const ShaderStruct SpaceTreeColorFragmentShader(R"glsl(

#version 450 core

in vec3 fColor;
out vec4 color;

void main()
{
    color = vec4( fColor, 1.0 );
}

)glsl",

{"color"}, {4}, {0}, {0},
{}, {} );


}
}

#endif