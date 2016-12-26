#version 430 core

uniform sampler2D tiledLights;

out vec4 outColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / textureSize(tiledLights, 0);
    outColor = texture(tiledLights, uv);
}