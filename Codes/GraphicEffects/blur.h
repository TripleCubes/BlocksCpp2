#ifndef BLUR_H
#define BLUR_H

#include "../Graphics/texture.h"
#include "../Graphics/shader.h"
#include "../Graphics/mesh.h"
#include "../Graphics/frameBuffer.h"

namespace GraphicEffects
{
    class Blur
    {
        private:
            static Shader shader;
            static Mesh mesh;
            static FrameBuffer reducedSizeFrameBufferMultisampled;
            static FrameBuffer reducedSizeFrameBuffer;

        public:
            static void init();
            static void createBlurTexture(Texture texture);
            static void createBlurTexture(unsigned int textureId);
            static void draw();
            static void release();
    };
}

#endif