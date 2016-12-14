#version 430 core

uniform mat4 mvp;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 10) out vec3 outPos;
layout(location = 11) out vec3 outNormal;

void main()
{
    gl_Position = mvp * vec4(inPos, 1.0);
    outPos = inPos;
    outNormal = inNormal;
    //outUV = inUV;
}