#version 330 core

out vec4 outputColor;
in vec2 fragmentInputTextureCoords;

uniform int horizontal = 1;
uniform int blurSize = 1;
uniform sampler2D texture;

void main()
{
    vec2 stepSize = vec2(0, 0);
    vec2 textureWH = textureSize(texture, 0);

    if (horizontal == 1)
    {
        stepSize = vec2(1/textureWH.x, 0);
    }
    else
    {
        stepSize = vec2(0, 1/textureWH.y);
    }

    vec3 result = vec3(0, 0, 0);
    for (int i = -blurSize; i <= blurSize; i++)
    {
        result += texture(texture, fragmentInputTextureCoords + stepSize*i).xyz;
    }
    result /= blurSize*2+1;

    outputColor = vec4(result, 1.0);
}