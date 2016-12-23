#version 430 core

uniform sampler2D gbufferPos;
uniform sampler2D gbufferNormal;
uniform sampler2D gbufferAlbedo;
uniform int lightsCount;
uniform vec3 lightsPos[32];
uniform vec3 lightsIntensity[32];

out vec3 outColor;

void main()
{
    vec2 fragCoord = gl_FragCoord.xy / textureSize(gbufferPos, 0);

    vec3 pos = texture(gbufferPos, fragCoord).xyz;
    vec3 normal = texture(gbufferNormal, fragCoord).xyz;
    vec3 albedo = texture(gbufferAlbedo, fragCoord).xyz;

    outColor = vec3(0,0,0);
    for (int i = 0; i < lightsCount; i++)
        outColor += max(0.0, dot(normal, normalize(lightsPos[i] - pos))) * albedo * lightsIntensity[i];
}