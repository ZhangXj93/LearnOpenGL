#version 330 core

in vec2 TexCoords;
in vec3 outNormal;
in vec3 outFragPos;

uniform vec3 viewPos;

out vec4 color;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
uniform Material material;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
void main()
{    
    vec3 normal = normalize(outNormal);
    vec3 viewDir = normalize(viewPos - outFragPos);

    vec3 finalColor = vec3(0.0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        finalColor += calculatePointLight(pointLights[i], normal, viewDir, outFragPos);
    }

    color = vec4(finalColor, 1.0);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 计算漫反射强度
    float diff = max(dot(normal, lightDir), 0.0);
    // 计算镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 计算衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 将各个分量合并
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}