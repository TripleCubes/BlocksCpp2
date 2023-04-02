#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <array>
#include <string>

#include "mesh.h"
#include "shader.h"
#include "vectormath.h"

const int CHUNK_SIZE = 16;

struct IntPos
{
    IntPos(int x, int y, int z);
    int x = 0;
    int y = 0;
    int z = 0;
    bool operator == (IntPos pos);
    IntPos chunkPos();
    IntPos blockChunkPos();
    std::string toString(bool endline = true);
    Vec3 toVec3();
};

enum BlockType
{
    EMPTY,
    TEST
};

struct Block
{
    Block(BlockType blockType, IntPos pos);
    Block();
    IntPos pos = IntPos(0, 0, 0);
    IntPos chunkPos = IntPos(0, 0, 0);
    IntPos blockChunkPos = IntPos(0, 0, 0);
    BlockType blockType = EMPTY;
};

class Chunk 
{
    private:
        enum Dir
        {
            TOP, BOTTOM, LEFT, RIGHT, FORWARD, BACKWARD
        };

        struct Surface
        {
            Dir dir;
            IntPos blockChunkPos;
            int w;
            int h;
        };

        std::array<std::array<std::array<Block, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> blocks;
        std::vector<float> verticies;
        std::vector<Surface> surfaceData;
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