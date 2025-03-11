#version 330 core
out vec4 FragColor;

in vec3 outNormal;
in vec3 outFragPos;
in vec2 TexCoords;
uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

uniform vec3 objectColor;

void main()
{
    // 环境光
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // 漫反射光
    vec3 lightDir = normalize(light.position - outFragPos);
    vec3 normal = normalize(outNormal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * light.diffuse;

    // 镜面光
    vec3 viewDir = normalize(viewPos - outFragPos);
    vec3 reflectDir = reflect(-lightDir, outNormal);
    float diff_spec = max(dot(viewDir, reflectDir), 0.0);
    float spec = pow(diff_spec, material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    // 衰减
    float distance = length(light.position - outFragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 finalColor = (diffuse + ambient + specular) * attenuation;

    FragColor = vec4(finalColor, 1.0);
}
