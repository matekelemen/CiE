defaultVertexShader = '''

varying vec3 position;
varying vec3 normal;
varying vec4 color;


void main() {
    normal      = $normal;
    color       = $color;
    position    = $position;
    gl_Position = $transform(vec4(position,1));
}

'''


defaultVertexShaderWithTexture = '''

varying vec3 position;
varying vec3 normal;
varying vec2 textureCoordinates;


void main() {
    normal              = $normal;
    textureCoordinates  = $textureCoordinates;
    position            = $position;
    gl_Position         = $transform(vec4(position,1));
}

'''