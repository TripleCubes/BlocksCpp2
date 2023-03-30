#include "chunkLoader.h"
#include "math.h"
#include "globals.h"
#include "threadControls.h"

int ChunkLoader::loadDistance = 1;
std::unordered_map<std::string, Chunk> ChunkLoader::chunks;
FastNoiseLite ChunkLoader::terrainHeightNoise;

void ChunkLoader::chunkLoadThreadFunction()
{
    IntPos playerChunkPos = IntPos(player.pos.x, player.pos.y, player.pos.z).chunkPos();

    for (int i = playerChunkPos.x - loadDistance; i <= playerChunkPos.x + loadDistance; i++)
    {
        for (int j = playerChunkPos.y - loadDistance; j <= playerChunkPos.y + loadDistance; j++)
        {
            for (int k = playerChunkPos.z - loadDistance; k <= playerChunkPos.z + loadDistance; k++)
            {
                if (!chunkLoaded(IntPos(i, j, k)))
                {
                    loadChunk(IntPos(i, j, k));
                }
            }
        }
    }
}

void ChunkLoader::setMeshesThreadFunction()
{
    for (std::unordered_map<std::string, Chunk>::iterator i = chunks.begin(); i != chunks.end(); i++)
    {
        if (!i->second.meshIsUpdated())
        {
            i->second.setMesh();
        }
    }
}

void ChunkLoader::chunkUnloadThreadFunction()
{
    IntPos playerChunkPos = IntPos(player.pos.x, player.pos.y, player.pos.z).chunkPos();

    for (std::unordered_map<std::string, Chunk>::iterator i = chunks.begin(); i != chunks.end();)
    {
        IntPos chunkPos = i->second.getChunkPos();
        if (abs(chunkPos.x - playerChunkPos.x) > loadDistance || abs(chunkPos.z - playerChunkPos.z) > loadDistance
        || abs(chunkPos.y - playerChunkPos.y) > loadDistance)
        {
            i = unloadChunk(chunkPos);
        }
        else
        {
            i++;
        }
    }
}

void ChunkLoader::init()
{
    terrainHeightNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    terrainHeightNoise.SetSeed(rand());
    terrainHeightNoise.SetFractalOctaves(3);
    terrainHeightNoise.SetFrequency(0.005);
}

std::string ChunkLoader::convertToKey(int x, int y, int z)
{
    std::string key = "";
    key += std::to_string(x);
    key += " ";
    key += std::to_string(y);
    key += " ";
    key += std::to_string(z);

    return key;
}

std::string ChunkLoader::convertToKey(IntPos pos)
{
    return convertToKey(pos.x, pos.y, pos.z);
}

Block ChunkLoader::getBlock(int x, int y, int z)
{
    return getBlock(IntPos(x, y, z));
}

Block ChunkLoader::getBlock(IntPos pos)
{
    if (chunkLoaded(pos.chunkPos()))
    {
        return chunks[convertToKey(pos.chunkPos())].getBlock(pos);
    }
    
    return Block(EMPTY, IntPos(0, 0, 0));
}

void ChunkLoader::placeBlock(Block block)
{
    std::string key = convertToKey(block.pos.chunkPos());
    chunks[key].addBlock(block);
    chunks[key].setMesh();
}

void ChunkLoader::breakBlock(IntPos pos)
{
    std::string key = convertToKey(pos.chunkPos());
    chunks[key].removeBlock(pos);
    chunks[key].setMesh();
}

bool ChunkLoader::chunkLoaded(IntPos chunkPos)
{
    return chunkLoaded(convertToKey(chunkPos));
}

bool ChunkLoader::chunkLoaded(std::string key)
{
    return chunks.find(key) != chunks.end();
}

void ChunkLoader::loadChunk(IntPos chunkPos)
{
    Chunk chunk(chunkPos);
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            int terrainHeight = round((terrainHeightNoise.GetNoise((float)(chunkPos.x * CHUNK_SIZE + x), 
                                                                (float)(chunkPos.z * CHUNK_SIZE + z))+1)/2 * 16) - 16;

            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                int worldY = chunkPos.y * CHUNK_SIZE + y;
                if (worldY <= terrainHeight)
                {
                    chunk.addBlock(Block(TEST, IntPos(x, y, z)));
                }
            }
        }
    }
    chunks.insert(std::make_pair(convertToKey(chunkPos), chunk));
}

std::unordered_map<std::string, Chunk>::iterator ChunkLoader::unloadChunk(IntPos chunkPos)
{
    std::string key = convertToKey(chunkPos);
    std::unordered_map<std::string, Chunk>::iterator i = chunks.find(key);

    if (i != chunks.end())
    {
        chunks[key].release();
        return chunks.erase(i);
    }

    return ++i;
}

void ChunkLoader::update()
{
}

void ChunkLoader::draw()
{
    for (std::unordered_map<std::string, Chunk>::iterator i = chunks.begin(); i != chunks.end(); i++)
    {
        i->second.draw();
    }
}

void ChunkLoader::release()
{
    for (std::unordered_map<std::string, Chunk>::iterator i = chunks.begin(); i != chunks.end(); i++)
    {
        i->second.release();
    }
}
