defaultFragmentShader = '''


varying vec3 position;
varying vec3 normal;
varying vec4 color;


void main() {

    // Diffuse
    vec3 lightDir       = normalize( position - vec3($lightPos) );
    float diffuse       = dot( normal, lightDir );
    diffuse             = min( max(diffuse,0.0), 1.0 );

    // Specular
    vec3 halfWayVector  = normalize(  ( lightDir+vec3($cameraDir) )/2.0  );
    float specular      = min( max(   dot(halfWayVector,normal)   ,0.0), 1.0 );
    specular            = specular * specular * specular * specular;
    specular            = specular * specular * specular * specular;
    specular            = specular * specular * specular * specular;

    // Additive
    vec3 fragColor      = vec3($lightColor) * color.xyz;
    fragColor           = fragColor * ( $diffuseMaterialConstant * diffuse +
                                        $specularMaterialConstant * specular)
                                        + $ambientMaterialConstant * vec3($ambientLight) * color.xyz;


    gl_FragColor        = vec4(fragColor,1.0);
}


'''