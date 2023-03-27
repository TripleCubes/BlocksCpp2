#include <stdio.h>

#include "chunk.h"
#include "math.h"
#include "globals.h"
#include "graphics.h"

IntPos::IntPos(int x, int y, int z): x{x}, y{y}, z{z} {}

bool IntPos::operator == (IntPos pos)
{
    if (x != pos.x || y != pos.y || z != pos.z)
    {
        return false;
    }
    return true;
}

IntPos IntPos::chunkPos()
{
    return IntPos((int)floor((float)x / CHUNK_SIZE), (int)floor((float)y / CHUNK_SIZE), (int)floor((float)z / CHUNK_SIZE));
}

IntPos IntPos::blockChunkPos()
{
    return IntPos(mod(x, CHUNK_SIZE), mod(y, CHUNK_SIZE), mod(z, CHUNK_SIZE));
}



Block::Block(BlockType blockType, IntPos pos): blockType{blockType}, pos{pos} {}
Block::Block(): blockType{EMPTY}, pos{IntPos(0, 0, 0)} {}



Chunk::Chunk(int x, int y, int z): pos(IntPos(x, y, z)) {}
Chunk::Chunk(): pos(IntPos(0, 0, 0)) {}

void Chunk::addBlock(Block block)
{
    blocks.push_back(block);
}

void Chunk::removeBlock(IntPos pos)
{
    for (int i = 0; i < blocks.size(); i++)
    {
        if (blocks[i].pos.blockChunkPos() == pos.blockChunkPos())
        {
            blocks.erase(blocks.begin() + i);
        }
    }
}

void Chunk::setMesh()
{
    verticies.clear();

    for (int i = 0; i < blocks.size(); i++)
    {
        Block block = blocks[i];

        std::vector<float> blockVerticies = {
            0 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,    1,    0,    0,    1, // A
            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,    1,    0,    1,    0, // C
            0 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,    1,    0,    0,    0, // D

            0 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,    1,    0,    0,    1, // A
            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,    1,    0,    1,    1, // B
            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,    1,    0,    1,    0, // C

            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,   -1,    0,    1,    1, // E
            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,   -1,    0,    1,    0, // H
            1 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,   -1,    0,    0,    0, // G

            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,   -1,    0,    1,    1, // E
            1 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,   -1,    0,    0,    0, // G
            1 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,   -1,    0,    0,    1, // F

            0 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,   -1,    0,    0,    0,    1, // A
            0 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,   -1,    0,    0,    1,    1, // D
            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,   -1,    0,    0,    1,    0, // H

            0 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,   -1,    0,    0,    0,    1, // A
            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,   -1,    0,    0,    1,    0, // H
            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,   -1,    0,    0,    0,    0, // E

            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    1,    0,    0,    1,    1, // B
            1 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    1,    0,    0,    0,    0, // G
            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    1,    0,    0,    0,    1, // C

            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    1,    0,    0,    1,    1, // B
            1 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    1,    0,    0,    1,    0, // F
            1 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    1,    0,    0,    0,    0, // G

            0 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,    0,    1,    0,    1, // D
            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,    0,    1,    1,    1, // C
            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,    0,    1,    0,    0, // H

            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,    0,    1,    1,    1, // C
            1 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,    0,    1,    1,    0, // G
            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    1 + (float)block.pos.blockChunkPos().z,    0,    0,    1,    0,    0, // H

            0 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,    0,   -1,    1,    1, // A
            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,    0,   -1,    1,    0, // E
            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,    0,   -1,    0,    1, // B

            1 + (float)block.pos.blockChunkPos().x,    1 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,    0,   -1,    0,    1, // B
            0 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,    0,   -1,    1,    0, // E
            1 + (float)block.pos.blockChunkPos().x,    0 + (float)block.pos.blockChunkPos().y,    0 + (float)block.pos.blockChunkPos().z,    0,    0,   -1,    0,    0, // F
        };

        verticies.insert(std::end(verticies), std::begin(blockVerticies), std::end(blockVerticies));
    }

    mesh.set(verticies);
}

void Chunk::draw()
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(pos.x*CHUNK_SIZE, pos.y*CHUNK_SIZE, pos.z*CHUNK_SIZE));

    Graphics::getViewShader().useProgram();
    Graphics::getViewShader().setUniform("modelMat", modelMat);
    Graphics::getViewShader().setUniform("testTexture", Graphics::testTexture, 0);

    mesh.draw();
}

Block Chunk::getBlock(int x, int y, int z)
{
    return getBlock(IntPos(x, y, z));
}

Block Chunk::getBlock(IntPos pos)
{
    for (int i = 0; i < blocks.size(); i++)
    {
        if (blocks[i].pos.blockChunkPos() == pos.blockChunkPos())
        {
            return blocks[i];
        }
    }

    return Block(EMPTY, IntPos(0, 0, 0));
}

void Chunk::release()
{
    mesh.release();
}