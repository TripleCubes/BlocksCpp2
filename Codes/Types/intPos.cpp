#include "intpos.h"
#include "../globalFunctions.h"
#include "../ChunkLoader/chunk.h"

IntPos::IntPos(int x, int y, int z): x{x}, y{y}, z{z} {}

bool IntPos::operator == (IntPos pos) const
{
    if (x != pos.x || y != pos.y || z != pos.z)
    {
        return false;
    }
    return true;
}

bool IntPos::operator != (IntPos pos) const
{
    if (x != pos.x || y != pos.y || z != pos.z)
    {
        return true;
    }
    return false;
}

IntPos IntPos::operator + (IntPos pos) const
{
    return IntPos(x + pos.x, y + pos.y, z + pos.z);
}

IntPos IntPos::operator - (IntPos pos) const
{
    return IntPos(x - pos.x, y - pos.y, z - pos.z);
}

IntPos IntPos::operator * (int num) const
{
    return IntPos(x * num, y * num, z * num);
}

IntPos IntPos::operator / (int num) const
{
    return IntPos(x / num, y / num, z / num);
}

IntPos IntPos::chunkPos() const
{
    return IntPos((int)floor((float)x / CHUNK_SIZE), (int)floor((float)y / CHUNK_SIZE), (int)floor((float)z / CHUNK_SIZE));
}

IntPos IntPos::blockChunkPos() const
{
    return IntPos(mod(x, CHUNK_SIZE), mod(y, CHUNK_SIZE), mod(z, CHUNK_SIZE));
}

std::string IntPos::toString(bool endline) const
{
    return std::to_string(x) +  " " + std::to_string(y) + " " + std::to_string(z) + (endline ? "\n" : "");
}

Vec3 IntPos::toVec3() const
{
    return Vec3((float)x, (float)y, (float)z);
}