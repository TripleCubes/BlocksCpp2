#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"
#include "shader.h"
#include "frameBuffer.h"
#include "texture.h"

class Graphics
{
    private:
        static glm::mat4 viewViewMat;
        static glm::mat4 viewProjectionMat;

        static Shader viewShader;
        static FrameBuffer viewFrameBuffer;

        static Shader screenShader;
        static Mesh screenMesh;

    public:
        static void init();
        static void update();
        static void draw();

        static glm::mat4 getViewViewMat();
        static glm::mat4 getViewProjectionMat();
        static Shader getViewShader();
        static FrameBuffer getViewFrameBuffer();

        static void release();

        static Texture testTexture;
};

#endif