#version 330 core
out vec4 FragColor;

in vec3 outNormal;
in vec3 outFragPos;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    // 环境光
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射光
    vec3 lightDir = normalize(lightPos - outFragPos);
    vec3 normal = normalize(outNormal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // 镜面光
    vec3 viewDir = normalize(viewPos - outFragPos);
    vec3 reflectDir = reflect(-lightDir, outNormal);
    float diff_spec = max(dot(viewDir, reflectDir), 0.0);
    float spec = pow(diff_spec, 32);
    float specularStrength = 0.5f;
    vec3 specular = specularStrength * spec * lightColor;

    vec3 finalColor = diffuse * objectColor + ambient * objectColor + specular * objectColor;

    FragColor = vec4(finalColor, 1.0);
}
