// attributes: position
// sizes: 3
// strides: 0 
// offsets: 0
// uniforms: 
// textures: 

#version 450 core

in vec3 position;

void main()
{
    gl_Position = vec4( position, 1.0 );
}