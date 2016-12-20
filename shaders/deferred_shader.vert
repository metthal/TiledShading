#version 430 core

uniform mat4 model;
uniform mat4 viewProject;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNormal;

void main()
{
    vec4 pos = model * vec4(inPos, 1.0);

    outPos = pos.xyz;
    outNormal = inNormal;

    gl_Position = viewProject * pos;
}