#version 430 core

layout(location = 10) in vec3 inPos;
layout(location = 11) in vec3 inNormal;

out vec3 fragColor;

void main()
{
    vec3 lightPos = vec3(5.0, 10.0, 0.0);
    vec3 lightDir = normalize(lightPos - inPos);
    fragColor = max(dot(lightDir, inNormal), 0.0) * vec3(1, 0, 0);
}