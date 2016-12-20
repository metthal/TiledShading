#version 430 core

uniform mat4 model;
uniform mat4 viewProject;

layout(location = 0) in vec3 inPos;

void main()
{
    gl_Position = viewProject * model * vec4(inPos, 1.0);
}