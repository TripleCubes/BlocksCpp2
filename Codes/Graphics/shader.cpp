#include <iostream>
#include "shader.h"
#include <glad/glad.h>

unsigned int compileShader(std::string path, ShaderType shaderType)
{
    std::ifstream file(path);
    if (file.fail())
    {
        std::cout << path << " not found" << std::endl;
        return -1;
    }
    std::string shaderCodeString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char *shaderCode = shaderCodeString.c_str();
    unsigned int shader;

    if (shaderType == ShaderType::VERTEX)
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "vertex shader compile error\n"
                      << path << std::endl
                      << infoLog << std::endl;
        }
    }
    else if (shaderType == ShaderType::FRAGMENT)
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "fragment shader compile error\n"
                      << path << std::endl
                      << infoLog << std::endl;
        }
    }

    return shader;
}

void linkShaderProgram(unsigned int shaderProgram, std::string vertexShaderPath, std::string fragmentShaderPath)
{
    unsigned int vertexShader = compileShader(vertexShaderPath, ShaderType::VERTEX);
    unsigned int fragmentShader = compileShader(fragmentShaderPath, ShaderType::FRAGMENT);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "shader linking error\n"
                  << vertexShaderPath << std::endl
                  << fragmentShaderPath << std::endl
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::init(std::string vertexShaderPath, std::string fragmentShaderPath)
{
    shaderProgram = glCreateProgram();
    linkShaderProgram(shaderProgram, vertexShaderPath, fragmentShaderPath);
}

void Shader::init(std::string shaderPath)
{
    shaderProgram = glCreateProgram();
    linkShaderProgram(shaderProgram, shaderPath + "Vertex.glsl", shaderPath + "Fragment.glsl");
}

unsigned int Shader::getShaderProgram()
{
    return shaderProgram;
}

void Shader::useProgram()
{
    glUseProgram(shaderProgram);
}

void Shader::setUniform(std::string uniformName, glm::mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniform(std::string uniformName, float x, float y)
{
    glUniform2f(glGetUniformLocation(shaderProgram, uniformName.c_str()), x, y);
}

void Shader::setUniform(std::string uniformName, Vec3 vec)
{
    glUniform3f(glGetUniformLocation(shaderProgram, uniformName.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setUniform(std::string uniformName, Color color)
{
    glUniform4f(glGetUniformLocation(shaderProgram, uniformName.c_str()), color.r, color.g, color.b, color.a);
}

void Shader::setUniform(std::string uniformName, int num)
{
    glUniform1i(glGetUniformLocation(shaderProgram, uniformName.c_str()), num);
}

void Shader::setTextureUniform(std::string uniformName, Texture texture, int textureUniformIndex) 
{
    setTextureUniform(uniformName, texture.getTexture(), textureUniformIndex);
}

void Shader::setTextureUniform(std::string uniformName, unsigned int texture, int textureUniformIndex)
{
    glUniform1i(glGetUniformLocation(shaderProgram, uniformName.c_str()), textureUniformIndex);
    glActiveTexture(GL_TEXTURE0 + textureUniformIndex);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::release()
{
    glDeleteProgram(shaderProgram);
}