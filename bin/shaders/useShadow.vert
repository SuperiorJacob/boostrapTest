// VERTEX SHADER - USE SHADOW
#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;

out vec4 vNormal;

uniform mat4 ProjectionViewModel;
uniform mat4 NormalMatrix;

void main()
{
    vNormal = NormalMatrix * Normal;
    gl_Position = ProjectionViewModel * Position;
}