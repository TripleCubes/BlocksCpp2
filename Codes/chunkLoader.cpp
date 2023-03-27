#include "chunkLoader.h"
#include "math.h"

std::unordered_map<std::string, Chunk> ChunkLoader::chunks;
FastNoiseLite ChunkLoader::terrainHeightNoise;

void ChunkLoader::init()
{
    terrainHeightNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    terrainHeightNoise.SetSeed(rand());
    terrainHeightNoise.SetFractalOctaves(3);
    terrainHeightNoise.SetFrequency(0.005);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Chunk chunk(i, 0, j);
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                for (int z = 0; z < CHUNK_SIZE; z++)
                {
                    chunk.addBlock(Block(TEST, 
                                            IntPos(x + i*CHUNK_SIZE, round((terrainHeightNoise.GetNoise((float)(x + i*CHUNK_SIZE), (float)(z + j*CHUNK_SIZE))+1)/2 * (CHUNK_SIZE - 1)), z + j*CHUNK_SIZE)));
                }
            }
            chunk.setMesh();
            chunks.insert(std::make_pair(convertToKey(i, 0, j), chunk));
        }
    }
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
    return chunks[convertToKey(pos.chunkPos())].getBlock(pos);
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
