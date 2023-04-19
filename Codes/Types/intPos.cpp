#include "intpos.h"
#include "../math.h"
#include "../ChunkLoader/chunk.h"

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