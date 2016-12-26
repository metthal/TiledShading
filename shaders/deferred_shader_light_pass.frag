#version 430 core

#define MAX_LIGHTS_COUNT 1024

struct Light
{
    float attenuation;
    float radius;
    vec3 position;
    vec3 intensity;
};

uniform vec3 cameraPos;
uniform sampler2D gbufferPos;
uniform sampler2D gbufferNormal;
uniform sampler2D gbufferAlbedo;
uniform sampler2D gbufferSpecular;
uniform int lightsCount;
layout(std140) uniform Lights
{
    Light lights[MAX_LIGHTS_COUNT];
};

out vec3 outColor;

float attenuation(vec3 lightPos, vec3 litPos, float lightAttenuation)
{
    float dist = distance(lightPos, litPos);
    float ret = 1.0 / (1.0 + lightAttenuation * dist * dist);
    return ret < 0.01 ? 0.0 : ret;
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
        vec3 lightDir = normalize(lights[i].position - pos);
        vec3 halfway = normalize(cameraDir + normal);

        vec3 diffuseColor = max(0.0, dot(normal, lightDir)) * albedo * lights[i].intensity;
        vec3 specularColor = pow(max(0.0, dot(halfway, lightDir)), 64) * specular * lights[i].intensity;

        float att = attenuation(lights[i].position, pos, lights[i].attenuation);
        outColor += att * (diffuseColor + specularColor);
    }
}