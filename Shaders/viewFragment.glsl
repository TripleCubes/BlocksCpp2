#version 330 core

in vec3 fragmentInputPos;
in vec3 fragmentInputNormal;
in vec2 fragmentInputTextureCoord;
out vec4 outputColor;

uniform sampler2D testTexture;

void main()
{
    vec3 result = texture(testTexture, fragmentInputTextureCoord).xyz;
    outputColor = vec4(result, 1.0f);
} 