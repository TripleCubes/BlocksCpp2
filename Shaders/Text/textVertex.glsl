#version 330 core

layout (location = 0) in vec2 vertexInputPos;

out vec2 fragmentInputTextureCoords;

uniform vec2 textPos;
uniform vec2 textSize;
uniform vec2 windowSize;

void main()
{
    vec2 resultPos = vec2(textPos.x/windowSize.x + vertexInputPos.x*textSize.x/windowSize.x,
                            textPos.y/windowSize.y + vertexInputPos.y*textSize.y/windowSize.y);
    resultPos = (resultPos * 2) - 1;
    gl_Position = vec4(resultPos, 0.0, 1.0);
    fragmentInputTextureCoords = vertexInputPos;
}