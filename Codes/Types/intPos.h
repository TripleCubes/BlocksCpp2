#ifndef INTPOS_H
#define INTPOS_H

#include <string>
#include "vectormath.h"

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

#endif