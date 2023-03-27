#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <unordered_map>
#include <string>

#include <FastNoiseLite.h>

#include "chunk.h"

class ChunkLoader 
{
    private:
        static std::unordered_map<std::string, Chunk> chunks;
        static FastNoiseLite terrainHeightNoise;

        static std::string convertToKey(int x, int y, int z);
        static std::string convertToKey(IntPos pos);

    public:
        static void init();

        static Block getBlock(int x, int y, int z);
        static Block getBlock(IntPos pos);
        static void placeBlock(Block block);
        static void breakBlock(IntPos pos);

        static void draw();

        static void release();
};

#endif