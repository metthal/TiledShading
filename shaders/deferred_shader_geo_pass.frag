#version 430 core

uniform vec3 objectColor;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

out vec3 outPos;
out vec3 outNormal;
out vec3 outAlbedo;

void main()
{
    outPos = inPos;
    outNormal = normalize(inNormal);
    outAlbedo = objectColor;
}