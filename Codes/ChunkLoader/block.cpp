#include "block.h"

Block::Block(BlockType blockType, IntPos pos): blockType{blockType}, pos{pos} {}
Block::Block(): blockType{EMPTY}, pos{IntPos(0, 0, 0)} {}