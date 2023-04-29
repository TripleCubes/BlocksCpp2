#ifndef BLOCK_H
#define BLOCK_H

#include "../Types/intPos.h"
#include <vector>

enum class BlockType
{
    EMPTY,
    STONE,
    DIRT,
    GRASS,
    LOG,
    LEAF,
};

const std::vector<BlockType> blocksWithAlpha = {
    BlockType::EMPTY, 
    BlockType::LEAF,
};

enum class BlockDir
{
    TOP, BOTTOM, LEFT, RIGHT, FORWARD, BACKWARD
};

struct Block
{
    Block(BlockType blockType, IntPos pos);
    Block();
    IntPos pos = IntPos(0, 0, 0);
    BlockType blockType = BlockType::EMPTY;
};

#endif