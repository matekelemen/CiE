// attributes: 
// sizes: 
// strides: 
// offsets: 
// uniforms: 
// textures: 

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