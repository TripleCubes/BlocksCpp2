#version 330 core

out vec4 outputColor;
in vec2 fragmentInputTextureCoords;

uniform vec4 rectColor;

void main()
{
    outputColor = rectColor;
}