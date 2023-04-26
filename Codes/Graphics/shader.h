#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>

#include "texture.h"
#include "../Types/vectormath.h"
#include "../Types/color.h"
#include "../Types/vec2.h"

class Shader
{
    private:
        unsigned int shaderProgram;
    
    public:
        void init(std::string vertexShaderPath, std::string fragmentShaderPath);
        void init(std::string shaderPath);
        unsigned int getShaderProgram();
        void useProgram();
        void setUniform(std::string uniformName, glm::mat4 mat);
        void setUniform(std::string uniformName, Vec3 vec);
        void setUniform(std::string uniformName, Vec2 vec);
        void setUniform(std::string uniformName, Color color);
        void setUniform(std::string uniformName, int num);
        void setTextureUniform(std::string uniformName, Texture texture, int textureUniformIndex);
        void setTextureUniform(std::string uniformName, unsigned int texture, int textureUniformIndex);
        void release();
};

enum class ShaderType 
{
    VERTEX,
    FRAGMENT
};

unsigned int compileShader(std::string path, ShaderType shader_type);
void linkShaderProgram(unsigned int shaderProgram, std::string vertexShaderPath, std::string fragmentShaderPath);

#endif