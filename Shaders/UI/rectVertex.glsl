#version 330 core

layout (location = 0) in vec2 vertexInputPos;

out vec2 fragmentInputTextureCoords;

uniform vec2 rectPos;
uniform vec2 rectSize;
uniform vec2 windowSize;

void main()
{
    vec2 resultPos = vec2(rectPos.x/windowSize.x + vertexInputPos.x*rectSize.x/windowSize.x,
                            rectPos.y/windowSize.y + vertexInputPos.y*rectSize.y/windowSize.y);
    resultPos = (resultPos * 2) - 1;
    gl_Position = vec4(resultPos, 0.0, 1.0);
    fragmentInputTextureCoords = (vertexInputPos + 1) / 2;
}