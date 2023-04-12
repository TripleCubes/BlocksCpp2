#version 330 core

out vec4 outputColor;
in vec2 fragmentInputTextureCoords;

uniform vec4 textColor;
uniform sampler2D textTexture;

void main()
{
    vec4 color = texture(textTexture, vec2(fragmentInputTextureCoords.x, 1 - fragmentInputTextureCoords.y));
    if (color.r < 0.5)
    {
        discard;
    }
    outputColor = textColor;
}