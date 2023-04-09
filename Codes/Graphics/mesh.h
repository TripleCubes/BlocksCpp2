#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Mesh
{
    private:
        unsigned int VAO;
        unsigned int EBO;
        bool VAOInitialized = false;
        bool EBOInitialized = false;
        int numberOfVerticies = 0;
        int numberOfIndicies = 0;

    public:
        void set(std::vector<float> verticies);
        void set(std::vector<float> verticies, std::vector<unsigned int> indicies);
        void set2d(std::vector<float> verticies);
        void set2d(std::vector<float> verticies, std::vector<unsigned int> indicies);
        void draw(bool drawLine = false);

        void release();
};

#endif