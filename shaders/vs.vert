#version 430 core

uniform mat4 mvp;

layout(location = 0) in vec3 inPos;
//layout(location = 1) in vec2 inUV;

//layout(location = 11) out vec2 outUV;

void main()
{
    gl_Position = mvp * vec4(inPos, 1.0);
    //outUV = inUV;
}