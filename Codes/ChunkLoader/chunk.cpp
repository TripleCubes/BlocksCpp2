#include <stdio.h>

#include "chunk.h"
#include "chunkLoader.h"
#include "../math.h"
#include "../globals.h"
#include "../Graphics/graphics.h"
#include "../Types/vec2.h"
#include "../GameTextures/blockTextures.h"

Chunk::Chunk(int x, int y, int z): chunkPos(IntPos(x, y, z)) {}
Chunk::Chunk(IntPos chunkPos): chunkPos(chunkPos) {}
Chunk::Chunk(): chunkPos(IntPos(0, 0, 0)) {}

IntPos Chunk::getChunkPos() const
{
    return chunkPos;
}

void Chunk::addBlock(Block block)
{
    surfaceDataUpdated = false;
    meshUpdated = false;

    blocks[block.pos.blockChunkPos().x][block.pos.blockChunkPos().y][block.pos.blockChunkPos().z] = block;
}

void Chunk::removeBlock(IntPos blockChunkPos)
{
    surfaceDataUpdated = false;
    meshUpdated = false;

    blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z] = Block(BlockType::EMPTY, IntPos(0, 0, 0));
}

Block Chunk::getBlock(IntPos blockChunkPos) const
{
    return blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z];
}

void Chunk::requestUpdateMesh()
{
    surfaceDataUpdated = false;
    meshUpdated = false;
}

void Chunk::updateSurfaceData()
{
    if (surfaceDataUpdated)
    {
        return;
    }

    struct BlockFace
    {
        BlockType blockType = BlockType::EMPTY;

        bool hasTopFace = false;
        bool hasBottomFace = false;
        bool hasLeftFace = false;
        bool hasRightFace = false;
        bool hasForwardFace = false;
        bool hasBackwardFace = false;

        bool topChecked = false;
        bool bottomChecked = false;
        bool leftChecked = false;
        bool rightChecked = false;
        bool forwardChecked = false;
        bool backwardChecked = false;
    };
    std::array<std::array<std::array<BlockFace, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> blockFaces;

    std::string topChunkKey         = ChunkLoader::convertToKey(IntPos(chunkPos.x, chunkPos.y + 1, chunkPos.z));
    std::string bottomChunkKey      = ChunkLoader::convertToKey(IntPos(chunkPos.x, chunkPos.y - 1, chunkPos.z));
    std::string leftChunkKey        = ChunkLoader::convertToKey(IntPos(chunkPos.x - 1, chunkPos.y, chunkPos.z));
    std::string rightChunkKey       = ChunkLoader::convertToKey(IntPos(chunkPos.x + 1, chunkPos.y, chunkPos.z));
    std::string forwardChunkKey     = ChunkLoader::convertToKey(IntPos(chunkPos.x, chunkPos.y, chunkPos.z + 1));
    std::string backwardChunkKey    = ChunkLoader::convertToKey(IntPos(chunkPos.x, chunkPos.y, chunkPos.z - 1));

    bool topChunkLoaded         = ChunkLoader::chunkLoaded(topChunkKey);
    bool bottomChunkLoaded      = ChunkLoader::chunkLoaded(bottomChunkKey);
    bool leftChunkLoaded        = ChunkLoader::chunkLoaded(leftChunkKey);
    bool rightChunkLoaded       = ChunkLoader::chunkLoaded(rightChunkKey);
    bool forwardChunkLoaded     = ChunkLoader::chunkLoaded(forwardChunkKey);
    bool backwardChunkLoaded    = ChunkLoader::chunkLoaded(backwardChunkKey);

    Chunk topChunk;
    Chunk bottomChunk;
    Chunk leftChunk;
    Chunk rightChunk;
    Chunk forwardChunk;
    Chunk backwardChunk;

    if (topChunkLoaded)
    {
        topChunk      = ChunkLoader::getChunk(topChunkKey);
    }
    if (bottomChunkLoaded)
    {
        bottomChunk   = ChunkLoader::getChunk(bottomChunkKey);
    }
    if (leftChunkLoaded)
    {
        leftChunk     = ChunkLoader::getChunk(leftChunkKey);
    }
    if (rightChunkLoaded)
    {
        rightChunk    = ChunkLoader::getChunk(rightChunkKey);
    }
    if (forwardChunkLoaded)
    {
        forwardChunk  = ChunkLoader::getChunk(forwardChunkKey);
    }
    if (backwardChunkLoaded)
    {
        backwardChunk = ChunkLoader::getChunk(backwardChunkKey);
    }

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                Block block = blocks[x][y][z];

                if (block.blockType != BlockType::EMPTY)
                {
                    if (y == CHUNK_SIZE - 1)
                    {
                        if (!topChunkLoaded || topChunk.getBlock(IntPos(x, 0, z)).blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasTopFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x][y + 1][z].blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasTopFace = true;
                        }
                    }

                    if (y == 0)
                    {
                        if (!bottomChunkLoaded || bottomChunk.getBlock(IntPos(x, CHUNK_SIZE - 1, z)).blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasBottomFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x][y - 1][z].blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasBottomFace = true;
                        }
                    }

                    if (x == 0)
                    {
                        if (!leftChunkLoaded || leftChunk.getBlock(IntPos(CHUNK_SIZE - 1, y, z)).blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasLeftFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x - 1][y][z].blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasLeftFace = true;
                        }
                    }

                    if (x == CHUNK_SIZE - 1)
                    {
                        if (!rightChunkLoaded || rightChunk.getBlock(IntPos(0, y, z)).blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasRightFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x + 1][y][z].blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasRightFace = true;
                        }
                    }

                    if (z == CHUNK_SIZE - 1)
                    {
                        if (!forwardChunkLoaded || forwardChunk.getBlock(IntPos(x, y, 0)).blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasForwardFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x][y][z + 1].blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasForwardFace = true;
                        }
                    }

                    if (z == 0)
                    {
                        if (!backwardChunkLoaded || backwardChunk.getBlock(IntPos(x, y, CHUNK_SIZE - 1)).blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasBackwardFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x][y][z - 1].blockType == BlockType::EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasBackwardFace = true;
                        }
                    }
                }
            }
        }
    }



    struct Surface
    {
        Vec2 uv;
        BlockDir dir;
        IntPos blockChunkPos;
        int w;
        int h;
    };

    std::vector<Surface> surfaceData;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (blockFaces[x][y][z].hasTopFace && !blockFaces[x][y][z].topChecked)
                {
                    BlockType blockType = blockFaces[x][y][z].blockType;
                    blockFaces[x][y][z].topChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (x + checkWidth <= CHUNK_SIZE - 1
                    && blockFaces[x + checkWidth][y][z].blockType == blockType
                    && blockFaces[x + checkWidth][y][z].hasTopFace 
                    && !blockFaces[x + checkWidth][y][z].topChecked)
                    {
                        blockFaces[x + checkWidth][y][z].topChecked = true;
                        checkWidth++;
                    }

                    bool stopHeightCheck = false;
                    while (!stopHeightCheck)
                    {
                        if (z + checkHeight >= CHUNK_SIZE)
                        {
                            break;
                        }

                        for (int i = 0; i < checkWidth; i++)
                        {
                            if (blockFaces[x + i][y][z + checkHeight].blockType != blockType
                            || !blockFaces[x + i][y][z + checkHeight].hasTopFace
                            || blockFaces[x + i][y][z + checkHeight].topChecked)
                            {
                                stopHeightCheck = true;
                                break;
                            }
                        }

                        if (!stopHeightCheck)
                        {
                            for (int i = 0; i < checkWidth; i++)
                            {
                                blockFaces[x + i][y][z + checkHeight].topChecked = true;
                            }
                            checkHeight++;
                        }
                    }

                    surfaceData.push_back({GameTextures::BlockTextures::getUV(blockFaces[x][y][z].blockType, BlockDir::TOP),
                                            BlockDir::TOP, IntPos(x, y, z), checkWidth, checkHeight});
                }
            }
        }
    }

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (blockFaces[x][y][z].hasBottomFace && !blockFaces[x][y][z].bottomChecked)
                {
                    BlockType blockType = blockFaces[x][y][z].blockType;
                    blockFaces[x][y][z].bottomChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (x + checkWidth <= CHUNK_SIZE - 1
                    && blockFaces[x + checkWidth][y][z].blockType == blockType
                    && blockFaces[x + checkWidth][y][z].hasBottomFace 
                    && !blockFaces[x + checkWidth][y][z].bottomChecked)
                    {
                        blockFaces[x + checkWidth][y][z].bottomChecked = true;
                        checkWidth++;
                    }

                    bool stopHeightCheck = false;
                    while (!stopHeightCheck)
                    {
                        if (z + checkHeight >= CHUNK_SIZE)
                        {
                            break;
                        }

                        for (int i = 0; i < checkWidth; i++)
                        {
                            if (blockFaces[x + i][y][z + checkHeight].blockType != blockType
                            || !blockFaces[x + i][y][z + checkHeight].hasBottomFace
                            || blockFaces[x + i][y][z + checkHeight].bottomChecked)
                            {
                                stopHeightCheck = true;
                                break;
                            }
                        }

                        if (!stopHeightCheck)
                        {
                            for (int i = 0; i < checkWidth; i++)
                            {
                                blockFaces[x + i][y][z + checkHeight].bottomChecked = true;
                            }
                            checkHeight++;
                        }
                    }

                    surfaceData.push_back({GameTextures::BlockTextures::getUV(blockFaces[x][y][z].blockType, BlockDir::BOTTOM),
                                            BlockDir::BOTTOM, IntPos(x, y, z), checkWidth, checkHeight});
                }
            }
        }
    }

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (blockFaces[x][y][z].hasLeftFace && !blockFaces[x][y][z].leftChecked)
                {
                    BlockType blockType = blockFaces[x][y][z].blockType;
                    blockFaces[x][y][z].leftChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (y + checkWidth <= CHUNK_SIZE - 1
                    && blockFaces[x][y + checkWidth][z].blockType == blockType
                    && blockFaces[x][y + checkWidth][z].hasLeftFace 
                    && !blockFaces[x][y + checkWidth][z].leftChecked)
                    {
                        blockFaces[x][y + checkWidth][z].leftChecked = true;
                        checkWidth++;
                    }

                    bool stopHeightCheck = false;
                    while (!stopHeightCheck)
                    {
                        if (z + checkHeight >= CHUNK_SIZE)
                        {
                            break;
                        }

                        for (int i = 0; i < checkWidth; i++)
                        {
                            if (blockFaces[x][y + i][z + checkHeight].blockType != blockType
                            || !blockFaces[x][y + i][z + checkHeight].hasLeftFace
                            || blockFaces[x][y + i][z + checkHeight].leftChecked)
                            {
                                stopHeightCheck = true;
                                break;
                            }
                        }

                        if (!stopHeightCheck)
                        {
                            for (int i = 0; i < checkWidth; i++)
                            {
                                blockFaces[x][y + i][z + checkHeight].leftChecked = true;
                            }
                            checkHeight++;
                        }
                    }

                    surfaceData.push_back({GameTextures::BlockTextures::getUV(blockFaces[x][y][z].blockType, BlockDir::LEFT),
                                            BlockDir::LEFT, IntPos(x, y, z), checkWidth, checkHeight});
                }
            }
        }
    }

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (blockFaces[x][y][z].hasRightFace && !blockFaces[x][y][z].rightChecked)
                {
                    BlockType blockType = blockFaces[x][y][z].blockType;
                    blockFaces[x][y][z].rightChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (y + checkWidth <= CHUNK_SIZE - 1
                    && blockFaces[x][y + checkWidth][z].blockType == blockType
                    && blockFaces[x][y + checkWidth][z].hasRightFace 
                    && !blockFaces[x][y + checkWidth][z].rightChecked)
                    {
                        blockFaces[x][y + checkWidth][z].rightChecked = true;
                        checkWidth++;
                    }

                    bool stopHeightCheck = false;
                    while (!stopHeightCheck)
                    {
                        if (z + checkHeight >= CHUNK_SIZE)
                        {
                            break;
                        }

                        for (int i = 0; i < checkWidth; i++)
                        {
                            if (blockFaces[x][y + i][z + checkHeight].blockType != blockType
                            || !blockFaces[x][y + i][z + checkHeight].hasRightFace
                            || blockFaces[x][y + i][z + checkHeight].rightChecked)
                            {
                                stopHeightCheck = true;
                                break;
                            }
                        }

                        if (!stopHeightCheck)
                        {
                            for (int i = 0; i < checkWidth; i++)
                            {
                                blockFaces[x][y + i][z + checkHeight].rightChecked = true;
                            }
                            checkHeight++;
                        }
                    }

                    surfaceData.push_back({GameTextures::BlockTextures::getUV(blockFaces[x][y][z].blockType, BlockDir::RIGHT),
                                            BlockDir::RIGHT, IntPos(x, y, z), checkWidth, checkHeight});
                }
            }
        }
    }

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (blockFaces[x][y][z].hasForwardFace && !blockFaces[x][y][z].forwardChecked)
                {
                    BlockType blockType = blockFaces[x][y][z].blockType;
                    blockFaces[x][y][z].forwardChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (x + checkWidth <= CHUNK_SIZE - 1
                    && blockFaces[x + checkWidth][y][z].blockType == blockType
                    && blockFaces[x + checkWidth][y][z].hasForwardFace 
                    && !blockFaces[x + checkWidth][y][z].forwardChecked)
                    {
                        blockFaces[x + checkWidth][y][z].forwardChecked = true;
                        checkWidth++;
                    }

                    bool stopHeightCheck = false;
                    while (!stopHeightCheck)
                    {
                        if (x + checkHeight >= CHUNK_SIZE)
                        {
                            break;
                        }

                        for (int i = 0; i < checkWidth; i++)
                        {
                            if (blockFaces[x + i][y + checkHeight][z].blockType != blockType
                            || !blockFaces[x + i][y + checkHeight][z].hasForwardFace
                            || blockFaces[x + i][y + checkHeight][z].forwardChecked)
                            {
                                stopHeightCheck = true;
                                break;
                            }
                        }

                        if (!stopHeightCheck)
                        {
                            for (int i = 0; i < checkWidth; i++)
                            {
                                blockFaces[x + i][y + checkHeight][z].forwardChecked = true;
                            }
                            checkHeight++;
                        }
                    }

                    surfaceData.push_back({GameTextures::BlockTextures::getUV(blockFaces[x][y][z].blockType, BlockDir::FORWARD),
                                            BlockDir::FORWARD, IntPos(x, y, z), checkWidth, checkHeight});
                }
            }
        }
    }

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (blockFaces[x][y][z].hasBackwardFace && !blockFaces[x][y][z].backwardChecked)
                {
                    BlockType blockType = blockFaces[x][y][z].blockType;
                    blockFaces[x][y][z].backwardChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (x + checkWidth <= CHUNK_SIZE - 1
                    && blockFaces[x + checkWidth][y][z].blockType == blockType
                    && blockFaces[x + checkWidth][y][z].hasBackwardFace 
                    && !blockFaces[x + checkWidth][y][z].backwardChecked)
                    {
                        blockFaces[x + checkWidth][y][z].backwardChecked = true;
                        checkWidth++;
                    }

                    bool stopHeightCheck = false;
                    while (!stopHeightCheck)
                    {
                        if (x + checkHeight >= CHUNK_SIZE)
                        {
                            break;
                        }

                        for (int i = 0; i < checkWidth; i++)
                        {
                            if (blockFaces[x + i][y + checkHeight][z].blockType != blockType
                            || !blockFaces[x + i][y + checkHeight][z].hasBackwardFace
                            || blockFaces[x + i][y + checkHeight][z].backwardChecked)
                            {
                                stopHeightCheck = true;
                                break;
                            }
                        }

                        if (!stopHeightCheck)
                        {
                            for (int i = 0; i < checkWidth; i++)
                            {
                                blockFaces[x + i][y + checkHeight][z].backwardChecked = true;
                            }
                            checkHeight++;
                        }
                    }

                    surfaceData.push_back({GameTextures::BlockTextures::getUV(blockFaces[x][y][z].blockType, BlockDir::BACKWARD),
                                            BlockDir::BACKWARD, IntPos(x, y, z), checkWidth, checkHeight});
                }
            }
        }
    }
    


    verticies.clear();

    for (int i = 0; i < surfaceData.size(); i++)
    {
        Surface surface = surfaceData[i];
        Vec3 pos = surface.blockChunkPos.toVec3();
        Vec2 uvSize = Vec2(1.0f/(float)GameTextures::BlockTextures::getTexture().getWidth(),
                            1.0f/(float)GameTextures::BlockTextures::getTexture().getHeight());
        uvSize = uvSize * (float)GameTextures::BlockTextures::blockPixelSize;
        float w = surface.uv.x + (float)surface.w * uvSize.x;
        float h = surface.uv.y + (float)surface.h * uvSize.y;
        float w2 = surface.uv.x + (float)surface.h * uvSize.x;
        float h2 = surface.uv.y + (float)surface.w * uvSize.y;

        std::vector<float> surfaceVerticies;
        
        if (surface.dir == BlockDir::TOP)
        {
            surfaceVerticies = {
                               0 + pos.x,  1 + pos.y,                 0 + pos.z,  0,  1,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,  surface.uv.y, // A
                (float)surface.w + pos.x,  1 + pos.y,  (float)surface.h + pos.z,  0,  1,  0,  surface.uv.x,  surface.uv.y,             w,             h, // C
                               0 + pos.x,  1 + pos.y,  (float)surface.h + pos.z,  0,  1,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,             h, // D

                               0 + pos.x,  1 + pos.y,                 0 + pos.z,  0,  1,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,  surface.uv.y, // A
                (float)surface.w + pos.x,  1 + pos.y,                 0 + pos.z,  0,  1,  0,  surface.uv.x,  surface.uv.y,             w,  surface.uv.y, // B
                (float)surface.w + pos.x,  1 + pos.y,  (float)surface.h + pos.z,  0,  1,  0,  surface.uv.x,  surface.uv.y,             w,             h, // C
            };
        }
        else if (surface.dir == BlockDir::BOTTOM)
        {
            surfaceVerticies = {
                               0 + pos.x,  0 + pos.y,                 0 + pos.z,  0, -1,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,             h, // E
                               0 + pos.x,  0 + pos.y,  (float)surface.h + pos.z,  0, -1,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,  surface.uv.y, // H
                (float)surface.w + pos.x,  0 + pos.y,  (float)surface.h + pos.z,  0, -1,  0,  surface.uv.x,  surface.uv.y,             w,  surface.uv.y, // G

                               0 + pos.x,  0 + pos.y,                 0 + pos.z,  0, -1,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,             h, // E
                (float)surface.w + pos.x,  0 + pos.y,  (float)surface.h + pos.z,  0, -1,  0,  surface.uv.x,  surface.uv.y,             w,  surface.uv.y, // G
                (float)surface.w + pos.x,  0 + pos.y,                 0 + pos.z,  0, -1,  0,  surface.uv.x,  surface.uv.y,             w,             h, // F
            };
        }
        else if (surface.dir == BlockDir::LEFT)
        {
            surfaceVerticies = {
                0 + pos.x,  (float)surface.w + pos.y,                 0 + pos.z, -1,  0,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,  surface.uv.y, // A
                0 + pos.x,  (float)surface.w + pos.y,  (float)surface.h + pos.z, -1,  0,  0,  surface.uv.x,  surface.uv.y,            w2,  surface.uv.y, // D
                0 + pos.x,                 0 + pos.y,  (float)surface.h + pos.z, -1,  0,  0,  surface.uv.x,  surface.uv.y,            w2,            h2, // H

                0 + pos.x,  (float)surface.w + pos.y,                 0 + pos.z, -1,  0,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,  surface.uv.y, // A
                0 + pos.x,                 0 + pos.y,  (float)surface.h + pos.z, -1,  0,  0,  surface.uv.x,  surface.uv.y,            w2,            h2, // H
                0 + pos.x,                 0 + pos.y,                 0 + pos.z, -1,  0,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,            h2, // E
            };
        }
        else if (surface.dir == BlockDir::RIGHT)
        {
            surfaceVerticies = {
                1 + pos.x,  (float)surface.w + pos.y,                 0 + pos.z,  1,  0,  0,  surface.uv.x,  surface.uv.y,            w2,  surface.uv.y, // B
                1 + pos.x,                 0 + pos.y,  (float)surface.h + pos.z,  1,  0,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,            h2, // G
                1 + pos.x,  (float)surface.w + pos.y,  (float)surface.h + pos.z,  1,  0,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,  surface.uv.y, // C

                1 + pos.x,  (float)surface.w + pos.y,                 0 + pos.z,  1,  0,  0,  surface.uv.x,  surface.uv.y,            w2,  surface.uv.y, // B
                1 + pos.x,                 0 + pos.y,                 0 + pos.z,  1,  0,  0,  surface.uv.x,  surface.uv.y,            w2,            h2, // F
                1 + pos.x,                 0 + pos.y,  (float)surface.h + pos.z,  1,  0,  0,  surface.uv.x,  surface.uv.y,  surface.uv.x,            h2, // G
            };
        }
        else if (surface.dir == BlockDir::FORWARD)
        {
            surfaceVerticies = {
                               0 + pos.x,  (float)surface.h + pos.y,  1 + pos.z,  0,  0,  1,  surface.uv.x,  surface.uv.y,  surface.uv.x,  surface.uv.y, // D
                (float)surface.w + pos.x,  (float)surface.h + pos.y,  1 + pos.z,  0,  0,  1,  surface.uv.x,  surface.uv.y,             w,  surface.uv.y, // C
                               0 + pos.x,                 0 + pos.y,  1 + pos.z,  0,  0,  1,  surface.uv.x,  surface.uv.y,  surface.uv.x,             h, // H

                (float)surface.w + pos.x,  (float)surface.h + pos.y,  1 + pos.z,  0,  0,  1,  surface.uv.x,  surface.uv.y,             w,  surface.uv.y, // C
                (float)surface.w + pos.x,                 0 + pos.y,  1 + pos.z,  0,  0,  1,  surface.uv.x,  surface.uv.y,             w,             h, // G
                               0 + pos.x,                 0 + pos.y,  1 + pos.z,  0,  0,  1,  surface.uv.x,  surface.uv.y,  surface.uv.x,             h, // H
            };
        }

        else if (surface.dir == BlockDir::BACKWARD)
        {
            surfaceVerticies = {
                               0 + pos.x,  (float)surface.h + pos.y,  0 + pos.z,  0,  0, -1,  surface.uv.x,  surface.uv.y,             w,  surface.uv.y, // A
                               0 + pos.x,                 0 + pos.y,  0 + pos.z,  0,  0, -1,  surface.uv.x,  surface.uv.y,             w,             h, // E
                (float)surface.w + pos.x,  (float)surface.h + pos.y,  0 + pos.z,  0,  0, -1,  surface.uv.x,  surface.uv.y,  surface.uv.x,  surface.uv.y, // B

                (float)surface.w + pos.x,  (float)surface.h + pos.y,  0 + pos.z,  0,  0, -1,  surface.uv.x,  surface.uv.y,  surface.uv.x,  surface.uv.y, // B
                               0 + pos.x,                 0 + pos.y,  0 + pos.z,  0,  0, -1,  surface.uv.x,  surface.uv.y,             w,             h, // E
                (float)surface.w + pos.x,                 0 + pos.y,  0 + pos.z,  0,  0, -1,  surface.uv.x,  surface.uv.y,  surface.uv.x,             h, // F
            };
        }

        verticies.insert(std::end(verticies), std::begin(surfaceVerticies), std::end(surfaceVerticies));
    }

    surfaceDataUpdated = true;
}

void Chunk::updateMesh()
{
    if (surfaceDataUpdated && !meshUpdated)
    {
        mesh.setBlockMesh(verticies);
        
        meshInitialized = true;
        meshUpdated = true;
    }
}

void Chunk::draw() const
{
    if (meshInitialized)
    {
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(chunkPos.x*CHUNK_SIZE, chunkPos.y*CHUNK_SIZE, chunkPos.z*CHUNK_SIZE));

        Graphics::getBlockShader().useProgram();
        Graphics::getBlockShader().setUniform("modelMat", modelMat);
        Graphics::getBlockShader().setTextureUniform("texture", GameTextures::BlockTextures::getTexture(), 0);

        mesh.draw();
    }
}

void Chunk::markBasePainted()
{
    isBasePainted = true;
}

void Chunk::markTopPainted()
{
    isTopPainted = true;
}

bool Chunk::basePainted()
{
    return isBasePainted;
}

bool Chunk::topPainted()
{
    return isTopPainted;
}

void Chunk::release()
{
    mesh.release();
}