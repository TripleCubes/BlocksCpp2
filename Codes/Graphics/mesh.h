#ifndef MESH_H
#define MESH_H

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
        bool drawLine = false;

    public:
        void setBlockMesh(std::vector<float> verticies);
        void set3d(std::vector<float> verticies, bool drawLine = false);
        void set3d(std::vector<float> verticies, std::vector<unsigned int> indicies, bool drawLine = false);
        void set2d(std::vector<float> verticies, bool drawLine = false);
        void set2d(std::vector<float> verticies, std::vector<unsigned int> indicies, bool drawLine = false);
        void draw();

        void release();
};

#endif