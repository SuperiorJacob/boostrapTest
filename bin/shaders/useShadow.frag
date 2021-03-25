// FRAGMENT SHADER - USE SHADOW
#version 410

in vec4 vNormal;
out vec4 FragColour;

uniform vec3 lightDirection;

void main()
{
    vec3 N = normalize(vNormal.xyz);
    float d = max(0, dot(N, -lightDirection));

    FragColour = vec4(d, d, d, 1);
}