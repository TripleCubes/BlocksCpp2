#version 330 core

in vec3 fragmentInputPos;
in vec3 fragmentInputNormal;
in vec2 fragmentInputTextureCoord;
out vec4 outputColor;

uniform sampler2D testTexture;

vec3 lightDir = vec3(0.4, 1, 0.4);

void main()
{
    float light = max(0.2, dot(lightDir, fragmentInputNormal));
    vec3 result = texture(testTexture, fragmentInputTextureCoord).xyz;
    result *= light;
    outputColor = vec4(result, 1.0f);
} 