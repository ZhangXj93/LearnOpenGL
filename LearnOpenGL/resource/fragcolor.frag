#version 330 core
out vec4 FragColor;

in vec2 outTexture;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
//    FragColor = texture(ourTexture, outTexture);
    FragColor = mix(texture(texture1, outTexture), texture(texture2, outTexture), 0.2);
}
