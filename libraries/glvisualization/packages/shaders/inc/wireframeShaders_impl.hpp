#ifndef GLVISUALIZATION_WIREFRAME_SHADERS_IMPL_HPP
#define GLVISUALIZATION_WIREFRAME_SHADERS_IMPL_HPP

namespace cie {
namespace gl {

const ShaderStruct wireframeGeometryShader(
R"glsl(

#version 450 core

layout(triangles) in;
layout(line_strip, max_vertices=4 ) out;

void main()
{
    for( int i=0; i<4; ++i )
    {
        gl_Position = gl_in[i%3].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}

)glsl",

{}, {}, {}, {},
{}, {} );


const ShaderStruct wireframeColorGeometryShader(
R"glsl(

#version 450 core

layout(triangles) in;
layout(line_strip, max_vertices=4 ) out;

in vec3 vColor[];
out vec3 fColor;

void main()
{
    for( int i=0; i<4; ++i )
    {
        fColor      = vColor[i%3];
        gl_Position = gl_in[i%3].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}

)glsl",

{}, {}, {}, {},
{}, {} );


}
}

#endif