#include <stdio.h>

#include "chunk.h"
#include "chunkLoader.h"
#include "../math.h"
#include "../globals.h"
#include "../Graphics/graphics.h"

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

std::string IntPos::toString(bool endline)
{
    return std::to_string(x) +  " " + std::to_string(y) + " " + std::to_string(z) + (endline ? "\n" : "");
}

Vec3 IntPos::toVec3()
{
    return Vec3((float)x, (float)y, (float)z);
}



Block::Block(BlockType blockType, IntPos pos): blockType{blockType}, pos{pos}, 
                                                chunkPos{pos.chunkPos()}, 
                                                blockChunkPos{pos.blockChunkPos()} {}
Block::Block(): blockType{EMPTY}, pos{IntPos(0, 0, 0)} {}



Chunk::Chunk(int x, int y, int z): chunkPos(IntPos(x, y, z)) {}
Chunk::Chunk(IntPos chunkPos): chunkPos(chunkPos) {}
Chunk::Chunk(): chunkPos(IntPos(0, 0, 0)) {}

IntPos Chunk::getChunkPos()
{
    return chunkPos;
}

void Chunk::addBlock(Block block)
{
    surfaceDataUpdated = false;
    meshUpdated = false;

    blocks[block.blockChunkPos.x][block.blockChunkPos.y][block.blockChunkPos.z] = block;
}

void Chunk::removeBlock(IntPos blockChunkPos)
{
    surfaceDataUpdated = false;
    meshUpdated = false;

    blocks[blockChunkPos.x][blockChunkPos.y][blockChunkPos.z] = Block(EMPTY, IntPos(0, 0, 0));
}

Block Chunk::getBlock(IntPos blockChunkPos)
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
        BlockType blockType = EMPTY;

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

                if (block.blockType != EMPTY)
                {
                    if (y == CHUNK_SIZE - 1)
                    {
                        if (!topChunkLoaded || topChunk.getBlock(IntPos(x, 0, z)).blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasTopFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x][y + 1][z].blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasTopFace = true;
                        }
                    }

                    if (y == 0)
                    {
                        if (!bottomChunkLoaded || bottomChunk.getBlock(IntPos(x, CHUNK_SIZE - 1, z)).blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasBottomFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x][y - 1][z].blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasBottomFace = true;
                        }
                    }

                    if (x == 0)
                    {
                        if (!leftChunkLoaded || leftChunk.getBlock(IntPos(CHUNK_SIZE - 1, y, z)).blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasLeftFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x - 1][y][z].blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasLeftFace = true;
                        }
                    }

                    if (x == CHUNK_SIZE - 1)
                    {
                        if (!rightChunkLoaded || rightChunk.getBlock(IntPos(0, y, z)).blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasRightFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x + 1][y][z].blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasRightFace = true;
                        }
                    }

                    if (z == CHUNK_SIZE - 1)
                    {
                        if (!forwardChunkLoaded || forwardChunk.getBlock(IntPos(x, y, 0)).blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasForwardFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x][y][z + 1].blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasForwardFace = true;
                        }
                    }

                    if (z == 0)
                    {
                        if (!backwardChunkLoaded || backwardChunk.getBlock(IntPos(x, y, CHUNK_SIZE - 1)).blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasBackwardFace = true;
                        }
                    }
                    else
                    {
                        if (blocks[x][y][z - 1].blockType == EMPTY)
                        {
                            blockFaces[x][y][z].blockType = block.blockType;
                            blockFaces[x][y][z].hasBackwardFace = true;
                        }
                    }
                }
            }
        }
    }



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

    std::vector<Surface> surfaceData;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (blockFaces[x][y][z].hasTopFace && !blockFaces[x][y][z].topChecked)
                {
                    blockFaces[x][y][z].topChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (blockFaces[x + checkWidth][y][z].hasTopFace 
                    && !blockFaces[x + checkWidth][y][z].topChecked
                    && x + checkWidth <= CHUNK_SIZE - 1)
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
                            if (!blockFaces[x + i][y][z + checkHeight].hasTopFace
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

                    surfaceData.push_back({TOP, IntPos(x, y, z), checkWidth, checkHeight});
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
                    blockFaces[x][y][z].bottomChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (blockFaces[x + checkWidth][y][z].hasBottomFace 
                    && !blockFaces[x + checkWidth][y][z].bottomChecked
                    && x + checkWidth <= CHUNK_SIZE - 1)
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
                            if (!blockFaces[x + i][y][z + checkHeight].hasBottomFace
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

                    surfaceData.push_back({BOTTOM, IntPos(x, y, z), checkWidth, checkHeight});
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
                    blockFaces[x][y][z].leftChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (blockFaces[x][y + checkWidth][z].hasLeftFace 
                    && !blockFaces[x][y + checkWidth][z].leftChecked
                    && y + checkWidth <= CHUNK_SIZE - 1)
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
                            if (!blockFaces[x][y + i][z + checkHeight].hasLeftFace
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

                    surfaceData.push_back({LEFT, IntPos(x, y, z), checkWidth, checkHeight});
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
                    blockFaces[x][y][z].rightChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (blockFaces[x][y + checkWidth][z].hasRightFace 
                    && !blockFaces[x][y + checkWidth][z].rightChecked
                    && y + checkWidth <= CHUNK_SIZE - 1)
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
                            if (!blockFaces[x][y + i][z + checkHeight].hasRightFace
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

                    surfaceData.push_back({RIGHT, IntPos(x, y, z), checkWidth, checkHeight});
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
                    blockFaces[x][y][z].forwardChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (blockFaces[x + checkWidth][y][z].hasForwardFace 
                    && !blockFaces[x + checkWidth][y][z].forwardChecked
                    && x + checkWidth <= CHUNK_SIZE - 1)
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
                            if (!blockFaces[x + i][y + checkHeight][z].hasForwardFace
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

                    surfaceData.push_back({FORWARD, IntPos(x, y, z), checkWidth, checkHeight});
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
                    blockFaces[x][y][z].backwardChecked = true;
                    int checkWidth = 1;
                    int checkHeight = 1;

                    while (blockFaces[x + checkWidth][y][z].hasBackwardFace 
                    && !blockFaces[x + checkWidth][y][z].backwardChecked
                    && x + checkWidth <= CHUNK_SIZE - 1)
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
                            if (!blockFaces[x + i][y + checkHeight][z].hasBackwardFace
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

                    surfaceData.push_back({BACKWARD, IntPos(x, y, z), checkWidth, checkHeight});
                }
            }
        }
    }
    


    verticies.clear();

    for (int i = 0; i < surfaceData.size(); i++)
    {
        Surface surface = surfaceData[i];
        Vec3 pos = surface.blockChunkPos.toVec3();

        std::vector<float> surfaceVerticies;
        
        if (surface.dir == TOP)
        {
            surfaceVerticies = {
                               0 + pos.x,  1 + pos.y,                 0 + pos.z,  0,  1,  0,                 0,  (float)surface.h, // A
                (float)surface.w + pos.x,  1 + pos.y,  (float)surface.h + pos.z,  0,  1,  0,  (float)surface.w,                 0, // C
                               0 + pos.x,  1 + pos.y,  (float)surface.h + pos.z,  0,  1,  0,                 0,                 0, // D

                               0 + pos.x,  1 + pos.y,                 0 + pos.z,  0,  1,  0,                 0,  (float)surface.h, // A
                (float)surface.w + pos.x,  1 + pos.y,                 0 + pos.z,  0,  1,  0,  (float)surface.w,  (float)surface.h, // B
                (float)surface.w + pos.x,  1 + pos.y,  (float)surface.h + pos.z,  0,  1,  0,  (float)surface.w,                 0, // C
            };
        }
        else if (surface.dir == BOTTOM)
        {
            surfaceVerticies = {
                               0 + pos.x,  0 + pos.y,                 0 + pos.z,  0, -1,  0,  (float)surface.w,  (float)surface.h, // E
                               0 + pos.x,  0 + pos.y,  (float)surface.h + pos.z,  0, -1,  0,  (float)surface.w,                 0, // H
                (float)surface.w + pos.x,  0 + pos.y,  (float)surface.h + pos.z,  0, -1,  0,                 0,                 0, // G

                               0 + pos.x,  0 + pos.y,                 0 + pos.z,  0, -1,  0,  (float)surface.w,  (float)surface.h, // E
                (float)surface.w + pos.x,  0 + pos.y,  (float)surface.h + pos.z,  0, -1,  0,                 0,                 0, // G
                (float)surface.w + pos.x,  0 + pos.y,                 0 + pos.z,  0, -1,  0,                 0,  (float)surface.h, // F
            };
        }
        else if (surface.dir == LEFT)
        {
            surfaceVerticies = {
                0 + pos.x,  (float)surface.w + pos.y,                 0 + pos.z, -1,  0,  0,                 0,  (float)surface.w, // A
                0 + pos.x,  (float)surface.w + pos.y,  (float)surface.h + pos.z, -1,  0,  0,  (float)surface.h,  (float)surface.w, // D
                0 + pos.x,                 0 + pos.y,  (float)surface.h + pos.z, -1,  0,  0,  (float)surface.h,                 0, // H

                0 + pos.x,  (float)surface.w + pos.y,                 0 + pos.z, -1,  0,  0,                 0,  (float)surface.w, // A
                0 + pos.x,                 0 + pos.y,  (float)surface.h + pos.z, -1,  0,  0,  (float)surface.h,                 0, // H
                0 + pos.x,                 0 + pos.y,                 0 + pos.z, -1,  0,  0,                 0,                 0, // E
            };
        }
        else if (surface.dir == RIGHT)
        {
            surfaceVerticies = {
                1 + pos.x,  (float)surface.w + pos.y,                 0 + pos.z,  1,  0,  0,  (float)surface.h,  (float)surface.w, // B
                1 + pos.x,                 0 + pos.y,  (float)surface.h + pos.z,  1,  0,  0,                 0,                 0, // G
                1 + pos.x,  (float)surface.w + pos.y,  (float)surface.h + pos.z,  1,  0,  0,                 0,  (float)surface.w, // C

                1 + pos.x,  (float)surface.w + pos.y,                 0 + pos.z,  1,  0,  0,  (float)surface.h,  (float)surface.w, // B
                1 + pos.x,                 0 + pos.y,                 0 + pos.z,  1,  0,  0,  (float)surface.h,                 0, // F
                1 + pos.x,                 0 + pos.y,  (float)surface.h + pos.z,  1,  0,  0,                 0,                 0, // G
            };
        }
        else if (surface.dir == FORWARD)
        {
            surfaceVerticies = {
                               0 + pos.x,  (float)surface.h + pos.y,  1 + pos.z,  0,  0,  1,                 0,  (float)surface.h, // D
                (float)surface.w + pos.x,  (float)surface.h + pos.y,  1 + pos.z,  0,  0,  1,  (float)surface.w,  (float)surface.h, // C
                               0 + pos.x,                 0 + pos.y,  1 + pos.z,  0,  0,  1,                 0,                 0, // H

                (float)surface.w + pos.x,  (float)surface.h + pos.y,  1 + pos.z,  0,  0,  1,  (float)surface.w,  (float)surface.h, // C
                (float)surface.w + pos.x,                 0 + pos.y,  1 + pos.z,  0,  0,  1,  (float)surface.w,                 0, // G
                               0 + pos.x,                 0 + pos.y,  1 + pos.z,  0,  0,  1,                 0,                 0, // H
            };
        }

        else if (surface.dir == BACKWARD)
        {
            surfaceVerticies = {
                               0 + pos.x,  (float)surface.h + pos.y,  0 + pos.z,  0,  0, -1,  (float)surface.w,  (float)surface.h, // A
                               0 + pos.x,                 0 + pos.y,  0 + pos.z,  0,  0, -1,  (float)surface.w,                 0, // E
                (float)surface.w + pos.x,  (float)surface.h + pos.y,  0 + pos.z,  0,  0, -1,                 0,  (float)surface.h, // B

                (float)surface.w + pos.x,  (float)surface.h + pos.y,  0 + pos.z,  0,  0, -1,                 0,  (float)surface.h, // B
                               0 + pos.x,                 0 + pos.y,  0 + pos.z,  0,  0, -1,  (float)surface.w,                 0, // E
                (float)surface.w + pos.x,                 0 + pos.y,  0 + pos.z,  0,  0, -1,                 0,                 0, // F
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
        mesh.set(verticies);
        
        meshInitialized = true;
        meshUpdated = true;
    }
}

void Chunk::draw()
{
    if (meshInitialized)
    {
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(chunkPos.x*CHUNK_SIZE, chunkPos.y*CHUNK_SIZE, chunkPos.z*CHUNK_SIZE));

        Graphics::getViewShader().useProgram();
        Graphics::getViewShader().setUniform("modelMat", modelMat);
        Graphics::getViewShader().setTextureUniform("testTexture", Graphics::testTexture, 0);

        mesh.draw();
    }
}

void Chunk::release()
{
    mesh.release();
}