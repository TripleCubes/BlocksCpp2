#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <array>
#include <string>

#include "../Graphics/mesh.h"
#include "../Graphics/shader.h"
#include "../Types/vectormath.h"
#include "../Types/intPos.h"
#include "block.h"

const int CHUNK_SIZE = 16;

class Chunk 
{
    private:
        std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> blocks;
        std::vector<float> verticies;
        Mesh mesh;
        IntPos chunkPos;

        bool meshInitialized = false;
        bool meshUpdated = false;
        bool surfaceDataUpdated = false;

    public:
        Chunk(int x, int y, int z);
        Chunk(IntPos chunkPos);
        Chunk();

        IntPos getChunkPos();
        void addBlock(Block block);
        void removeBlock(IntPos blockChunkPos);
        Block getBlock(IntPos blockChunkPos);

        void requestUpdateMesh();
        void updateSurfaceData();
        void updateMesh();
        void draw();

        void release();
};

#endif