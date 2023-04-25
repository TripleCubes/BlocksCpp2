#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class FrameBuffer
{
    private:
        unsigned int FBO;
        unsigned int texture;
        unsigned int RBO;
        bool isMultisample = false;

    public:
        void init(bool multisample = false, bool linearFilter = false, int width = 0, int height = 0);
        void bind();
        unsigned int getFrameBufferObject();
        unsigned int getTexture();
        void resize(int width = 0, int height = 0);
        void release();
};

#endif