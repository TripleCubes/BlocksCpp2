#version 330 core

layout (location = 0) in vec3 vertexInputPos;
layout (location = 1) in vec3 vertexInputNormal;
layout (location = 2) in vec2 vertexInputTextureCoord;
out vec3 fragmentInputPos;
out vec3 fragmentInputNormal;
out vec2 fragmentInputTextureCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main()
{
    gl_Position = projectionMat * viewMat * modelMat * vec4(vertexInputPos.x, vertexInputPos.y, vertexInputPos.z, 1.0);
    // fragmentInputPos = (modelMat * vec4(vertexInputPos, 1.0)).xyz;
    fragmentInputPos = vertexInputPos;
    fragmentInputNormal = vertexInputNormal;
    fragmentInputTextureCoord = vertexInputTextureCoord;
}