#version 410

uniform mat4 matrix;
uniform mat4 perspective;
uniform mat3 normalMatrix;
uniform bool noColor;
uniform vec3 lightPosition;

// World coordinates
in vec4 vertex;
in vec4 normal;
in vec4 color;

// Camera-space coordinates
out vec4 eyeVector;
out vec4 lightVector;
out vec4 lightSpace; // placeholder for shadow mapping
out vec4 vertColor;
out vec4 vertNormal;

void main( void )
{
    //position de la camera, 1.0 pour que ce soit un point
    vec4 posCamera = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 posLight = matrix * vec4(lightPosition, 1.0);

    if (noColor) vertColor = vec4(0.2, 0.6, 0.7, 1.0 );
    else vertColor = color;
    vertNormal.xyz = normalize(normalMatrix * normal.xyz);
    vertNormal.w = 0.0;

    // TODO: compute eyeVector, lightVector.
    eyeVector= normalize(posCamera - matrix * vertex);

    lightVector = normalize(posLight - matrix * vertex);


    gl_Position = perspective * matrix * vertex;
}
