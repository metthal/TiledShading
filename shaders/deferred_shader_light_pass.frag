#version 430 core

uniform sampler2D gbufferPos;
uniform sampler2D gbufferNormal;
uniform sampler2D gbufferAlbedo;
uniform int lightsCount;
uniform vec3 lights[32];

out vec3 outColor;

void main()
{
    vec2 fragCoord = gl_FragCoord.xy / textureSize(gbufferPos, 0);

    vec3 pos = texture(gbufferPos, fragCoord).xyz;
    vec3 normal = texture(gbufferNormal, fragCoord).xyz;
    vec3 albedo = texture(gbufferAlbedo, fragCoord).xyz;
    outColor = max(0.0, dot(normal, normalize(lights[0] - pos))) * albedo * vec3(1,0,0);
}