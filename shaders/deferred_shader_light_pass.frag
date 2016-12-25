#version 430 core

uniform vec3 cameraPos;
uniform sampler2D gbufferPos;
uniform sampler2D gbufferNormal;
uniform sampler2D gbufferAlbedo;
uniform sampler2D gbufferSpecular;
uniform int lightsCount;
uniform vec3 lightsPos[64];
uniform vec3 lightsIntensity[64];
uniform float lightsAttenuation[64];

out vec3 outColor;

float attenuation(vec3 lightPos, vec3 litPos, float lightAttenuation)
{
    float dist = distance(lightPos, litPos);
    return 1.0 / (1.0 + lightAttenuation * dist * dist);
}

void main()
{
    vec2 fragCoord = gl_FragCoord.xy / textureSize(gbufferPos, 0);

    vec3 pos = texture(gbufferPos, fragCoord).xyz;
    vec3 normal = texture(gbufferNormal, fragCoord).xyz;
    vec3 albedo = texture(gbufferAlbedo, fragCoord).xyz;
    vec3 specular = texture(gbufferSpecular, fragCoord).xyz;

    vec3 cameraDir = normalize(cameraPos - pos);
    outColor = albedo * vec3(0.1, 0.1, 0.1);
    for (int i = 0; i < lightsCount; i++)
    {
        vec3 lightDir = normalize(lightsPos[i] - pos);
        vec3 halfway = normalize(cameraDir + normal);

        vec3 diffuseColor = max(0.0, dot(normal, lightDir)) * albedo * lightsIntensity[i];
        vec3 specularColor = pow(max(0.0, dot(halfway, lightDir)), 16) * specular * lightsIntensity[i];

        float att = attenuation(lightsPos[i], pos, lightsAttenuation[i]);
        outColor += att * (diffuseColor + specularColor);
    }
}