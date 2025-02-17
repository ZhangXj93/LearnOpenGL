#version 330 core
out vec4 FragColor;

in vec2 outTexture;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = vec4(lightColor * objectColor, 1.0);
}
