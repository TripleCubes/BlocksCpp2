#include "chunkLoader.h"
#include "terrain.h"
#include "../math.h"
#include "../globals.h"
#include "../threadControls.h"

int ChunkLoader::loadDistance = 8;
int ChunkLoader::chunkLoadPerCycle = 5;
std::unordered_map<std::string, Chunk> ChunkLoader::chunks;
bool ChunkLoader::unloadAllChunksRequested = false;

void ChunkLoader::chunkLoadThreadFunction()
{
    int numberOfChunkLoaded = 0;
    IntPos playerChunkPos = IntPos(player.pos.x, player.pos.y, player.pos.z).chunkPos();

    for (int distance = 0; distance <= loadDistance; distance++)
    {
        for (int i = playerChunkPos.x - distance; i <= playerChunkPos.x + distance; i++)
        {
            for (int j = playerChunkPos.y - distance; j <= playerChunkPos.y + distance; j++)
            {
                for (int k = playerChunkPos.z - distance; k <= playerChunkPos.z + distance; k++)
                {
                    if (!chunkLoaded(IntPos(i, j, k)))
                    {
                        loadChunk(IntPos(i, j, k));
                        numberOfChunkLoaded++;
                        if (numberOfChunkLoaded == chunkLoadPerCycle)
                        {
                            return;
                        }
                    }
                }
            }
        }
    }
}

void ChunkLoader::chunkPaintTopThreadFunction()
{
    for (std::unordered_map<std::string, Chunk>::iterator i = chunks.begin(); i != chunks.end(); i++)
    {
        if (!i->second.topPainted())
        {
            Terrain::paintTerrainTop(i->second);
        }
    }
}

void ChunkLoader::updateSurfaceDataThreadFunction()
{
    for (std::unordered_map<std::string, Chunk>::iterator i = chunks.begin(); i != chunks.end(); i++)
    {
        i->second.updateSurfaceData();
    }
}

void ChunkLoader::updateMeshesThreadFunction()
{
    for (std::unordered_map<std::string, Chunk>::iterator i = chunks.begin(); i != chunks.end(); i++)
    {
        i->second.updateMesh();
    }
}

void ChunkLoader::chunkUnloadThreadFunction()
{
    if (unloadAllChunksRequested)
    {
        unloadAllChunks();
        unloadAllChunksRequested = false;
        return;
    }

    IntPos playerChunkPos = IntPos(player.pos.x, player.pos.y, player.pos.z).chunkPos();

    for (std::unordered_map<std::string, Chunk>::iterator i = chunks.begin(); i != chunks.end();)
    {
        IntPos chunkPos = i->second.getChunkPos();
        if (abs(chunkPos.x - playerChunkPos.x) > loadDistance || abs(chunkPos.z - playerChunkPos.z) > loadDistance
        || abs(chunkPos.y - playerChunkPos.y) > loadDistance)
        {
            i->second.release();
            i = chunks.erase(i);
        }
        else
        {
            i++;
        }
    }
}

void ChunkLoader::unloadAllChunks()
{
    for (std::unordered_map<std::string, Chunk>::iterator i = chunks.begin(); i != chunks.end();)
    {
        IntPos chunkPos = i->second.getChunkPos();
        i = unloadChunk(chunkPos);
    }
}

void ChunkLoader::init()
{
    Terrain::init();
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

std::string ChunkLoader::convertToKey(IntPos chunkPos)
{
    return convertToKey(chunkPos.x, chunkPos.y, chunkPos.z);
}

Chunk &ChunkLoader::getChunk(IntPos chunkPos)
{
    return getChunk(convertToKey(chunkPos));
}

Chunk &ChunkLoader::getChunk(std::string key)
{
    return chunks[key];
}

Block ChunkLoader::getBlock(int x, int y, int z)
{
    return getBlock(IntPos(x, y, z));
}

Block ChunkLoader::getBlock(IntPos pos)
{
    if (chunkLoaded(pos.chunkPos()))
    {
        return chunks[convertToKey(pos.chunkPos())].getBlock(pos.blockChunkPos());
    }
    
    return Block(BlockType::EMPTY, IntPos(0, 0, 0));
}

void ChunkLoader::placeBlock(Block block)
{
    std::string key = convertToKey(block.pos.chunkPos());
    chunks[key].addBlock(block);
    requestUpdateChunksAround(block.pos.chunkPos());
}

void ChunkLoader::breakBlock(IntPos pos)
{
    std::string key = convertToKey(pos.chunkPos());
    chunks[key].removeBlock(pos.blockChunkPos());
    requestUpdateChunksAround(pos.chunkPos());
}

void ChunkLoader::requestUpdateChunksAround(IntPos chunkPos)
{
    std::string topChunkKey         = ChunkLoader::convertToKey(IntPos(chunkPos.x, chunkPos.y + 1, chunkPos.z));
    std::string bottomChunkKey      = ChunkLoader::convertToKey(IntPos(chunkPos.x, chunkPos.y - 1, chunkPos.z));
    std::string leftChunkKey        = ChunkLoader::convertToKey(IntPos(chunkPos.x - 1, chunkPos.y, chunkPos.z));
    std::string rightChunkKey       = ChunkLoader::convertToKey(IntPos(chunkPos.x + 1, chunkPos.y, chunkPos.z));
    std::string forwardChunkKey     = ChunkLoader::convertToKey(IntPos(chunkPos.x, chunkPos.y, chunkPos.z + 1));
    std::string backwardChunkKey    = ChunkLoader::convertToKey(IntPos(chunkPos.x, chunkPos.y, chunkPos.z - 1));

    bool topChunkLoaded         = ChunkLoader::chunkLoaded(topChunkKey);
    bool bottomChunkLoaded      = ChunkLoader::chunkLoaded(bottomChunkKey);
    bool leftChunkLoaded        = ChunkLoader::chunkLoaded(leftChunkKey);
    bool rightChunkLoaded       = ChunkLoader::chunkLoaded(rightChunkKey);
    bool forwardChunkLoaded     = ChunkLoader::chunkLoaded(forwardChunkKey);
    bool backwardChunkLoaded    = ChunkLoader::chunkLoaded(backwardChunkKey);

    if (topChunkLoaded)
    {
        chunks[topChunkKey].requestUpdateMesh();
    }
    if (bottomChunkLoaded)
    {
        chunks[bottomChunkKey].requestUpdateMesh();
    }
    if (leftChunkLoaded)
    {
        chunks[leftChunkKey].requestUpdateMesh();
    }
    if (rightChunkLoaded)
    {
        chunks[rightChunkKey].requestUpdateMesh();
    }
    if (forwardChunkLoaded)
    {
        chunks[forwardChunkKey].requestUpdateMesh();
    }
    if (backwardChunkLoaded)
    {
        chunks[backwardChunkKey].requestUpdateMesh();
    }
}

void ChunkLoader::requestUnloadAllChunks()
{
    unloadAllChunksRequested = true;
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
    for (int x = CHUNK_SIZE * chunkPos.x; x < CHUNK_SIZE * chunkPos.x + CHUNK_SIZE; x++)
    {
        for (int z = CHUNK_SIZE * chunkPos.z; z < CHUNK_SIZE * chunkPos.z + CHUNK_SIZE; z++)
        {
            for (int y = CHUNK_SIZE * chunkPos.y; y < CHUNK_SIZE * chunkPos.y + CHUNK_SIZE; y++)
            {
                Block block = Terrain::getBlock(IntPos(x, y, z));
                if (block.blockType != BlockType::EMPTY)
                {
                    chunk.addBlock(block);
                }
            }
        }
    }
    Terrain::paintTerrainBase(chunk);
    chunks.insert(std::make_pair(convertToKey(chunkPos), chunk));
    requestUpdateChunksAround(chunkPos);
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

void ChunkLoader::setLoadDistance(int loadDistance)
{
    ChunkLoader::loadDistance = loadDistance;
}

void ChunkLoader::update()
{
    chunkLoadThreadFunction();
    chunkPaintTopThreadFunction();
    updateSurfaceDataThreadFunction();
    updateMeshesThreadFunction();
    chunkUnloadThreadFunction();
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
