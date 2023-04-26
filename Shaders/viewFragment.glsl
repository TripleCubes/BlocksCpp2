#version 330 core

in vec3 fragmentInputPos;
in vec3 fragmentInputNormal;
in vec2 fragmentInputTextureCoord;
out vec4 outputColor;

uniform sampler2D texture;

vec3 lightDir = vec3(0.4, 1, 0.4);

void main()
{
    vec3 result = texture(texture, fragmentInputTextureCoord).xyz;

    float light = max(0.2, dot(lightDir, fragmentInputNormal));
    result *= light;

    outputColor = vec4(result, 1.0f);
} 