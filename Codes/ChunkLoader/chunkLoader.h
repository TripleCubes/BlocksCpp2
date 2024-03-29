#ifndef CHUNKLOADER_H
#define CHUNKLOADER_H

#include <unordered_map>
#include <string>

#include "chunk.h"

#include "../Types/intPos.h"

class ChunkLoader 
{
    private:
        static int loadDistance;
        static int chunkLoadPerCycle;

        static std::unordered_map<std::string, Chunk> chunks;

        static bool unloadAllChunksRequested;
        static void unloadAllChunks();

        static void loadAtDistance(int distance);

    public:
        static void init();

        static void chunkLoadThreadFunction();
        static void chunkPaintTopThreadFunction();
        static void chunkAddStructureThreadFunction();
        static void updateSurfaceDataThreadFunction();
        static void updateMeshesThreadFunction();
        static void chunkClearVerticiesDataThreadFunction();
        static void chunkUnloadThreadFunction();

        static std::string convertToKey(int x, int y, int z);
        static std::string convertToKey(IntPos chunkPos);

        static Chunk &getChunk(IntPos chunkPos);
        static Chunk &getChunk(const std::string &key);
        static Block getBlock(int x, int y, int z);
        static Block getBlock(IntPos pos);
        static void placeBlock(Block block);
        static void breakBlock(IntPos pos);
        static void requestUpdateChunksAround(IntPos chunkPos);
        static void requestUnloadAllChunks();

        static bool chunkLoaded(IntPos chunkPos);
        static bool chunkLoaded(const std::string &key);

        static void loadChunk(IntPos chunkPos);

        static void setLoadDistance(int loadDistance);
        static int getLoadDistance();

        static void update();
        static void draw();

        static void release();
};

#endif