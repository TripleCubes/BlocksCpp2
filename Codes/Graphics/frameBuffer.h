#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class FrameBuffer
{
    private:
        unsigned int FBO;
        unsigned int texture;
        unsigned int texture2;
        unsigned int RBO;
        bool isMultisample = false;

    public:
        void init(bool multisample, int numberOfTextures = 1);
        void bind();
        unsigned int getFrameBufferObject();
        unsigned int getTexture();
        unsigned int getTexture2();
        void resize();
        void release();
};

#endif