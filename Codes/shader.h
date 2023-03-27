#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>

#include "texture.h"

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
        void setUniform(std::string uniformName, float x, float y);
        void setUniform(std::string uniformName, Texture texture, int textureUniformIndex);
        void setUniform(std::string uniformName, unsigned int texture, int textureUniformIndex);
        void release();
};

enum ShaderType 
{
    VERTEX,
    FRAGMENT
};

unsigned int compileShader(std::string path, ShaderType shader_type);
void linkShaderProgram(unsigned int shaderProgram, std::string vertexShaderPath, std::string fragmentShaderPath);

#endif