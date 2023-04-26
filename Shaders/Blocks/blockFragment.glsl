#version 330 core

in vec3 fragmentInputPos;
in vec3 fragmentInputNormal;
in vec2 fragmentInputUVStartCoord;
in vec2 fragmentInputTextureCoord;
out vec4 outputColor;

uniform sampler2D texture;
uniform vec2 oneBlockTextureSize;

vec3 lightDir = vec3(0.4, 1, 0.4);

void main()
{
    vec2 textureCoord = fragmentInputUVStartCoord 
        + vec2(mod((fragmentInputTextureCoord.x - fragmentInputUVStartCoord.x), oneBlockTextureSize.x),
                mod((fragmentInputTextureCoord.y - fragmentInputUVStartCoord.y), oneBlockTextureSize.y));
    vec3 result = texture(texture, textureCoord).xyz;

    float light = max(0.2, dot(lightDir, fragmentInputNormal));
    result *= light;

    outputColor = vec4(result, 1.0f);
} 