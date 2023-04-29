#include "tree.h"
#include "../random.h"
#include "../ChunkLoader/chunkLoader.h"
#include "../ChunkLoader/terrain.h"
#include "../globalFunctions.h"
#include "structureLoader.h"
#include "../globals.h"
#include <array>

int Structure::Tree::checkRadius = 2;

std::unordered_map<std::string, IntPos> Structure::Tree::checkedChunks;

void Structure::Tree::checkLoad(IntPos chunk2dPos)
{
    for (int x = -checkRadius; x <= checkRadius; x++)
    {
        for (int z = -checkRadius; z <= checkRadius; z++)
        {
            std::string key = convertToKey(IntPos(chunk2dPos.x, 0, chunk2dPos.z) + IntPos(x, 0, z));
            if (checkedChunks.find(key) != checkedChunks.end())
            {
                continue;
            }

            checkedChunks.insert(std::make_pair(key, IntPos(chunk2dPos.x, 0, chunk2dPos.z) + IntPos(x, 0, z)));

            int numberOfTrees = Random::getInt(0, 2);
            for (int i = 0; i < numberOfTrees; i++)
            {
                IntPos treePos = IntPos(Random::getInt(0, CHUNK_SIZE), 0, Random::getInt(0, CHUNK_SIZE)) + (chunk2dPos + IntPos(x, 0, z)) * CHUNK_SIZE;
                treePos.y = Terrain::getTerrainHeight(treePos);

                int count = 0;
                while (count < 20)
                {
                    count++;

                    BlockType blockType = Terrain::getBlock(treePos).blockType;
                    
                    if (blockType == BlockType::EMPTY)
                    {
                        treePos.y--;
                        continue;
                    }

                    if (blockType == BlockType::GRASS || blockType == BlockType::DIRT)
                    {
                        treePos.y++;
                        Structure::Tree tree;
                        tree.init(treePos);
                        StructureLoader::add(tree);
                        break;
                    }
                }
            }
        }
    }
}

void Structure::Tree::removeCheckedMarks()
{
    for (std::unordered_map<std::string, IntPos>::iterator i = checkedChunks.begin(); i != checkedChunks.end();)
    {
        IntPos playerChunkPos = IntPos(player.pos.x, player.pos.y, player.pos.z).chunkPos();
        if (abs(i->second.x - playerChunkPos.x) > ChunkLoader::getLoadDistance() + checkRadius
        || abs(i->second.z - playerChunkPos.z) > ChunkLoader::getLoadDistance() + checkRadius)
        {
            i = checkedChunks.erase(i);
        }
        else
        {
            i++;
        }
    }
}

std::string Structure::Tree::convertToKey(IntPos chunk2dPos)
{
    std::string key = "";
    key += std::to_string(chunk2dPos.x);
    key += " ";
    key += std::to_string(chunk2dPos.z);

    return key;
}

void Structure::Tree::init(IntPos pos)
{
    IntPos mainBranchCursor = pos;
    int mainBranchHeight = Random::getInt(6, 8);
    for (int i = 0; i < mainBranchHeight; i++)
    {
        addBlock(Block(BlockType::LOG, mainBranchCursor));
        if (i < mainBranchHeight - 1)
        {
            mainBranchCursor = mainBranchCursor + IntPos(0, 1, 0);
            if (i > 1)
            {
                int rnd = Random::getInt(0, 5);
                if (rnd == 0)
                {
                    mainBranchCursor = mainBranchCursor + IntPos(Random::getInt(-1, 1), 0, 0);
                }
                else if (rnd == 1)
                {
                    mainBranchCursor = mainBranchCursor + IntPos(0, 0, Random::getInt(-1, 1));
                }
            }
        }
    }

    const int TOPLEFT = 0; 
    const int TOP = 1; 
    const int TOPRIGHT = 2;
    const int RIGHT = 3;
    const int BOTTOMRIGHT = 4;
    const int BOTTOM = 5;
    const int BOTTOMLEFT = 6;
    const int LEFT = 7;

    std::array<IntPos, 8> dirVectors = {
        IntPos(-1,  0, -1),
        IntPos( 0,  0, -1),
        IntPos( 1,  0, -1),
        IntPos( 1,  0,  0),
        IntPos( 1,  0,  1),
        IntPos( 0,  0,  1),
        IntPos(-1,  0,  1),
        IntPos(-1,  0,  0),
    };
    std::vector<int> branchDirs;
    int numberOfBranches = Random::getInt(2, 3);
    for (int i = 0; i < numberOfBranches; i++)
    {
        int dir = TOPLEFT;
        if (i == 0)
        {
            dir = Random::getInt(0, 7);
        }
        else if (i == 1)
        {
            dir = ringAdd(branchDirs[0], Random::getInt(3, 5));
        }
        else if (i == 2)
        {
            dir = ringAdd(ringMiddle(branchDirs[0], branchDirs[1]), Random::getInt(0, 1));
        }
        branchDirs.push_back(dir);

        int branchLength = Random::getInt(2, 3);
        IntPos cursor = mainBranchCursor;
        if (i != 0 && Random::dice(1, 2))
        {
            cursor = cursor + IntPos(0, Random::getInt(-1, 1), 0);
        }
        for (int j = 0; j < branchLength; j++)
        {
            cursor = cursor + dirVectors[dir];
            if (Random::dice(1, 3))
            {
                cursor = cursor + IntPos(0, 1, 0);
            }
            addBlock(Block(BlockType::LOG, cursor));
        }

        int leafRadius = Random::getInt(3, 4);
        for (int x = -leafRadius; x <= leafRadius; x++)
        {
            for (int z = -leafRadius; z <= leafRadius; z++)
            {
                for (int y = 0; y <= leafRadius; y++)
                {
                    if (x*x + z*z + (y-1)*(y-1)*Random::getInt(3, 4) < (leafRadius)*(leafRadius)
                    && !blockExist(cursor + IntPos(x, y, z)))
                    {
                        addBlock(Block(BlockType::LEAF, cursor + IntPos(x, y, z)));
                    }
                }
                
                if (Random::dice(1, 2))
                {
                    if (x*x + z*z + Random::getInt(3, 4) < (leafRadius - 1)*(leafRadius - 1)
                    && !blockExist(cursor + IntPos(x, -1, z)))
                    {
                        addBlock(Block(BlockType::LEAF, cursor + IntPos(x, -1, z)));
                    }
                }
            }
        }
    }
}

bool Structure::Tree::blockExist(IntPos pos)
{
    for (int i = 0; i < blockList.size(); i++)
    {
        for (int j = 0; j < blockList[i].size(); j++)
        {
            if (blockList[i][j].pos == pos)
            {
                return true;
            }
        }
    }

    return false;
}

int Structure::Tree::ringAdd(int a, int b)
{
    return mod(a + b, 8);
}

int Structure::Tree::ringMiddle(int a, int b)
{
    if (abs(b - a) > 4)
    {
        return (b + a) / 2;
    }

    return ringAdd((b + a)/2, 4);
}

void Structure::Tree::addBlock(Block block)
{
    int chunkIndex = findInList<IntPos>(belongToChunks, block.pos.chunkPos());
    if (chunkIndex == -1)
    {
        chunkIndex = belongToChunks.size();
        belongToChunks.push_back(block.pos.chunkPos());
        blockList.push_back(std::vector<Block>());
    }
    blockList[chunkIndex].push_back(block);
}

void Structure::Tree::place()
{
    for (int i = blockList.size() - 1; i >= 0 ; i--)
    {
        place(ChunkLoader::getChunk(belongToChunks[i]));
    }
}

bool Structure::Tree::place(Chunk &chunk)
{
    int chunkIndex = findInList<IntPos>(belongToChunks, chunk.getChunkPos());
    if (chunkIndex == -1)
    {
        return false;
    }

    for (int i = 0; i < blockList[chunkIndex].size(); i++)
    {
        chunk.addBlock(blockList[chunkIndex][i]);
    }

    belongToChunks.erase(belongToChunks.begin() + chunkIndex);
    blockList.erase(blockList.begin() + chunkIndex);
    if (belongToChunks.size() == 0)
    {
        return true;
    }
    return false;
}
