#version 330 core

layout (location = 0) in vec2 vertexInputPos;

out vec2 fragmentInputTextureCoords;

void main()
{
    gl_Position = vec4(vertexInputPos, 0.0, 1.0);
    fragmentInputTextureCoords = (vertexInputPos + 1) / 2;
}