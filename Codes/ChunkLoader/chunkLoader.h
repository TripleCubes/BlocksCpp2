#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <unordered_map>
#include <string>

#include <FastNoiseLite.h>

#include "chunk.h"

class ChunkLoader 
{
    private:
        static int loadDistance;
        static int chunkLoadPerCycle;

        static std::unordered_map<std::string, Chunk> chunks;
        static FastNoiseLite terrainHeightNoise;

    public:
        static void init();

        static void chunkLoadThreadFunction();
        static void updateSurfaceDataThreadFunction();
        static void updateMeshesThreadFunction();
        static void chunkUnloadThreadFunction();

        static std::string convertToKey(int x, int y, int z);
        static std::string convertToKey(IntPos chunkPos);

        static Chunk getChunk(IntPos chunkPos);
        static Chunk getChunk(std::string key);
        static Block getBlock(int x, int y, int z);
        static Block getBlock(IntPos pos);
        static void placeBlock(Block block);
        static void breakBlock(IntPos pos);
        static void requestUpdateChunksAround(IntPos chunkPos);

        static bool chunkLoaded(IntPos chunkPos);
        static bool chunkLoaded(std::string key);

        static void loadChunk(IntPos chunkPos);
        static std::unordered_map<std::string, Chunk>::iterator unloadChunk(IntPos chunkPos);

        static void update();
        static void draw();

        static void release();
};

#endif