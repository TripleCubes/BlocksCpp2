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
            static FrameBuffer horizontalBlurFrameBuffer;
            static FrameBuffer blurFrameBuffer;

            static void createBlurTexture1Time(unsigned int textureId);

        public:
            static void init();
            static void createBlurTexture(Texture texture, int blurSize = 1, int blurTimes = 1);
            static void createBlurTexture(unsigned int textureId, int blurSize = 1, int blurTimes = 1);
            static void draw();
            static void release();
    };
}

#endif