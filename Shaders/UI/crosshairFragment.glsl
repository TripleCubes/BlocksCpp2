#version 330 core

out vec4 outputColor;
in vec2 fragmentInputTextureCoords;

uniform sampler2D viewTexture;

void main()
{
    vec4 fragmentTextureColor = texture(viewTexture, fragmentInputTextureCoords);
    outputColor = vec4(vec3(1.0, 1.0, 1.0) - fragmentTextureColor.xyz, 1.0);
}