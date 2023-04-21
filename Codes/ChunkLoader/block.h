#ifndef BLOCK_H
#define BLOCK_H

#include "../Types/intPos.h"

enum class BlockType
{
    EMPTY,
    TEST
};

struct Block
{
    Block(BlockType blockType, IntPos pos);
    Block();
    IntPos pos = IntPos(0, 0, 0);
    BlockType blockType = BlockType::EMPTY;
};

#endif