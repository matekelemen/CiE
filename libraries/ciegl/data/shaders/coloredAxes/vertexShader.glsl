#version 450 core

// --- Input Declarations ---
in vec3 position;
in vec4 color;

// --- Output Declarations ---
out vec4 vColor;

// --- Uniform Declarations ---
uniform mat4 transformation;


void main()
{
    vColor = color;
    gl_Position = vec4( transformation * vec4(position, 1.0) );
}