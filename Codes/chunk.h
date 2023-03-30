#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
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
    BlockType blockType = EMPTY;
};

class Chunk 
{
    private:
        std::vector<Block> blocks;
        std::vector<float> verticies;
        Mesh mesh;
        bool meshUpdated = false;
        IntPos chunkPos;

    public:
        Chunk(int x, int y, int z);
        Chunk(IntPos chunkPos);
        Chunk();
        IntPos getChunkPos();
        void addBlock(Block block);
        void removeBlock(IntPos pos);
        void setMesh();
        bool meshIsUpdated();
        Block getBlock(int x, int y, int z);
        Block getBlock(IntPos pos);
        void draw();
        void release();
};

#endif