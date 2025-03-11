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
    vec3 direction;  // 光源方向
    float cutOff;    // 切光角（内圆锥角度的余弦值）
    float outerCutOff; // 外圆锥角度的余弦值
};
uniform Light light;

void main()
{
    // 环境光
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
    vec3 lightDir = normalize(light.position - outFragPos);

    float theta = dot(lightDir, -light.direction);
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // 漫反射光
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
    vec3 finalColor = ambient + (diffuse + specular) * attenuation * intensity;
    FragColor = vec4(finalColor, 1.0);
}
