#ifndef BLOCKTEXTURES_H
#define BLOCKTEXTURES_H
#include "../ChunkLoader/block.h"
#include "../Graphics/texture.h"
#include "../Types/vec2.h"

namespace GameTextures {
    class BlockTextures
    {
        private:
            static Texture texture;

        public:
            static void init();
            static Texture getTexture();
            static Vec2 getUV(BlockType blockType, BlockDir blockDir);
            static void release();

            static const int blockPixelSize;
    };
}

#endif