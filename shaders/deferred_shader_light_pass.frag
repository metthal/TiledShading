#version 430 core

uniform vec3 lightPos;
uniform sampler2D gbufferPos;
uniform sampler2D gbufferNormal;
uniform sampler2D gbufferAlbedo;

out vec3 outColor;

void main()
{
    vec2 fragCoord = gl_FragCoord.xy / vec2(800, 600);

    vec3 pos = texture(gbufferPos, fragCoord).xyz;
    vec3 normal = texture(gbufferNormal, fragCoord).xyz;
    vec3 albedo = texture(gbufferAlbedo, fragCoord).xyz;

    outColor = dot(normal, normalize(lightPos - pos)) * albedo * vec3(1,0,0);
}