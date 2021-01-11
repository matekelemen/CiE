#version 450 core

// --- Layout Definition ---
layout(triangles) in;
layout(triangle_strip, max_vertices=3 ) out;

// --- Input Declarations ---
in vec4 vColor[];

// --- Output Declarations ---
out vec4 fColor;


void main()
{
    for ( int i=0; i<3; ++i )
    {
        fColor      = vColor[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}