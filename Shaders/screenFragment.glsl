#version 330 core

out vec4 outputColor;
in vec2 fragmentInputTextureCoords;

uniform sampler2D viewTexture;

void main()
{
    vec3 result = texture(viewTexture, fragmentInputTextureCoords).xyz;

    outputColor = vec4(result, 1.0);
}