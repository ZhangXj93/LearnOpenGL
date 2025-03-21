#version 330 core

in vec2 TexCoords;
uniform sampler2D texture1;

out vec4 color;

float LinearizeDepth(float depth) {
    float near = 0.1;
    float far = 100.0;
    float z = depth * 2.0 - 1.0;  // 归一化设备坐标还原
    return (2.0 * near) / (far + near - z * (far - near));
}

void main()
{    
    color = texture(texture1, TexCoords);
    // float depth = LinearizeDepth(gl_FragCoord.z);
    // color = vec4(vec3(depth), 1.0f);
}