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
    bool operator == (IntPos pos) const;
    bool operator != (IntPos pos) const;
    IntPos operator + (IntPos pos) const;
    IntPos operator - (IntPos pos) const;
    IntPos operator * (int num) const;
    IntPos operator / (int num) const;
    IntPos chunkPos() const;
    IntPos blockChunkPos() const;
    std::string toString(bool endline = true) const;
    Vec3 toVec3() const;
};

#endif