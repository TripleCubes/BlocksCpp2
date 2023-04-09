#ifndef RAYCAST_H
#define RAYCAST_H

#include "../Types/vectormath.h"
#include "../ChunkLoader/chunk.h"

struct BlockRaycast
{
    BlockRaycast(Vec3 from, Vec3 dir, float range);
    BlockRaycast();
    bool found = false;
    Block selectedBlock;
    IntPos selectedBlockPos = IntPos(0, 0, 0);
    IntPos blockPlacingPos = IntPos(0, 0, 0);
};

#endif