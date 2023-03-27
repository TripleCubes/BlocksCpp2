#version 330 core

layout (location = 0) in vec2 vertexInputPos;
out vec2 fragmentInputTextureCoords;
uniform vec2 currentWindowSize;

void main()
{
    vec2 resultPos;
    resultPos.x = 2.0 / currentWindowSize.x * vertexInputPos.x;
    resultPos.y = 2.0 / currentWindowSize.y * vertexInputPos.y;
    gl_Position = vec4(resultPos, 0.0, 1.0);
    fragmentInputTextureCoords = (resultPos + 1) / 2;
}