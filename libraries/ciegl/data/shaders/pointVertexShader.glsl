// attributes: position
// sizes: 3
// strides: 0
// offsets: 0
// uniforms: transformation
// textures: 

#version 450 core

// Vertex layout:
//  poition      : x, y, z


in vec3     position;

uniform mat4 transformation;

void main()
{
    gl_Position = transformation * vec4( position, 1.0 );
}