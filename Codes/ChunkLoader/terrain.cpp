#include "terrain.h"
#include "../Types/vec2.h"
#include "chunkLoader.h"
#include "chunk.h"
#include <vector>

#include "../random.h"

FastNoiseLite Terrain::terrainShape_noise;
CurveMap Terrain::terrain_curveMap;

FastNoiseLite Terrain::bigMountains_noise;
CurveMap Terrain::bigMountains_curveMap;

FastNoiseLite Terrain::smallMountains_noise;
CurveMap Terrain::smallMountains_curveMap;

int Terrain::terrainHeight_max = 80;

void Terrain::init()
{
    terrainShape_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    terrainShape_noise.SetSeed(rand());
    terrainShape_noise.SetFractalOctaves(5);
    terrainShape_noise.SetFrequency(0.03);
    terrainShape_noise.SetFractalLacunarity(1.5);

    std::vector<Vec2> terrain_curveMap_points = {
        Vec2(0.000000, 0.000000),
        Vec2(0.009335, 0.001004),
        Vec2(0.035026, 0.003861),
        Vec2(0.073604, 0.008335),
        Vec2(0.121600, 0.014191),
        Vec2(0.175543, 0.021196),
        Vec2(0.231965, 0.029113),
        Vec2(0.287396, 0.037709),
        Vec2(0.338365, 0.046748),
        Vec2(0.381404, 0.055996),
        Vec2(0.413043, 0.065217),
        Vec2(0.435622, 0.077909),
        Vec2(0.454365, 0.096835),
        Vec2(0.470135, 0.120665),
        Vec2(0.483791, 0.148070),
        Vec2(0.496196, 0.177717),
        Vec2(0.508209, 0.208278),
        Vec2(0.520691, 0.238422),
        Vec2(0.534504, 0.266817),
        Vec2(0.550509, 0.292135),
        Vec2(0.569565, 0.313043),
        Vec2(0.590335, 0.329817),
        Vec2(0.611026, 0.343930),
        Vec2(0.631822, 0.355852),
        Vec2(0.652904, 0.366052),
        Vec2(0.674457, 0.375000),
        Vec2(0.696661, 0.383165),
        Vec2(0.719700, 0.391017),
        Vec2(0.743757, 0.399026),
        Vec2(0.769013, 0.407661),
        Vec2(0.795652, 0.417391),
        Vec2(0.821148, 0.435178),
        Vec2(0.843096, 0.465774),
        Vec2(0.861861, 0.506335),
        Vec2(0.877809, 0.554017),
        Vec2(0.891304, 0.605978),
        Vec2(0.902713, 0.659374),
        Vec2(0.912400, 0.711361),
        Vec2(0.920730, 0.759096),
        Vec2(0.928070, 0.799735),
        Vec2(0.934783, 0.830435),
        Vec2(0.941891, 0.855257),
        Vec2(0.949913, 0.879791),
        Vec2(0.958457, 0.903491),
        Vec2(0.967130, 0.925809),
        Vec2(0.975543, 0.946196),
        Vec2(0.983304, 0.964104),
        Vec2(0.990022, 0.978987),
        Vec2(0.995304, 0.990296),
        Vec2(1.000000, 1.000000),
    };
    terrain_curveMap.setPoints(terrain_curveMap_points);

    bigMountains_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    bigMountains_noise.SetSeed(rand());
    bigMountains_noise.SetFractalOctaves(1);
    bigMountains_noise.SetFrequency(0.002);

    std::vector<Vec2> bigMountains_curveMap_points = {
        Vec2(0.000000, 0.000000),
        Vec2(0.017381, 0.000756),
        Vec2(0.064800, 0.003050),
        Vec2(0.135169, 0.006919),
        Vec2(0.221400, 0.012400),
        Vec2(0.316406, 0.019531),
        Vec2(0.413100, 0.028350),
        Vec2(0.504394, 0.038894),
        Vec2(0.583200, 0.051200),
        Vec2(0.642431, 0.065306),
        Vec2(0.675000, 0.081250),
        Vec2(0.691012, 0.108225),
        Vec2(0.705600, 0.153050),
        Vec2(0.719213, 0.212200),
        Vec2(0.732300, 0.282150),
        Vec2(0.745312, 0.359375),
        Vec2(0.758700, 0.440350),
        Vec2(0.772913, 0.521550),
        Vec2(0.788400, 0.599450),
        Vec2(0.805613, 0.670525),
        Vec2(0.825000, 0.731250),
        Vec2(0.846606, 0.782819),
        Vec2(0.869600, 0.828800),
        Vec2(0.893119, 0.869231),
        Vec2(0.916300, 0.904150),
        Vec2(0.938281, 0.933594),
        Vec2(0.958200, 0.957600),
        Vec2(0.975194, 0.976206),
        Vec2(0.988400, 0.989450),
        Vec2(1.000000, 1.000000),
    };
    bigMountains_curveMap.setPoints(bigMountains_curveMap_points);

    smallMountains_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    smallMountains_noise.SetSeed(rand());
    smallMountains_noise.SetFractalOctaves(3);
    smallMountains_noise.SetFrequency(0.005);
}

int Terrain::getTerrainHeight(IntPos block2dPos)
{
    float bigMountains_height = (bigMountains_noise.GetNoise((float)block2dPos.x, (float)block2dPos.z)+1)/2;
    bigMountains_height = bigMountains_curveMap.map(bigMountains_height);
    float smallMountains_height = (smallMountains_noise.GetNoise((float)block2dPos.x, (float)block2dPos.z)+1)/2;
    smallMountains_height = smallMountains_curveMap.map(smallMountains_height);
    return (int)round((smallMountains_height * 0.3 + bigMountains_height * 0.7) * terrainHeight_max);
}

Block Terrain::getBlock(IntPos blockPos)
{
    float terrain_noiseValue = (terrainShape_noise.GetNoise((float)blockPos.x, (float)blockPos.y, (float)blockPos.z)+1)/2;
    int maxMountainsHeight = getTerrainHeight(blockPos);
    int minMountainsHeight = -10;
    float terrainShape_noiseValueFilter = ((float)blockPos.y-(float)minMountainsHeight)/((float)maxMountainsHeight-(float)minMountainsHeight);
    if (terrainShape_noiseValueFilter <= 0)
    {
        return Block(BlockType::STONE, blockPos);
    }
    else if (terrainShape_noiseValueFilter >= 1)
    {
        return Block(BlockType::EMPTY, IntPos(0, 0, 0));
    }
    else
    {
        terrainShape_noiseValueFilter = terrain_curveMap.map(terrainShape_noiseValueFilter);
        if (terrain_noiseValue > terrainShape_noiseValueFilter)
        {
            if (maxMountainsHeight - blockPos.y < 25)
            {
                if (terrain_noiseValue - terrainShape_noiseValueFilter < 0.2f / pow((float)(maxMountainsHeight - blockPos.y)/25.0f, 0.2f))
                {
                    return Block(BlockType::DIRT, blockPos);
                }
            }
            return Block(BlockType::STONE, blockPos);
        }
    }

    return Block(BlockType::EMPTY, IntPos(0, 0, 0));
}

void Terrain::paintTerrainBase(Chunk &chunk)
{
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_SIZE - 1; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                if (chunk.getBlock(IntPos(x, y, z)).blockType == BlockType::DIRT
                && chunk.getBlock(IntPos(x, y + 1, z)).blockType == BlockType::EMPTY)
                {
                    IntPos blockPos = chunk.getBlock(IntPos(x, y, z)).pos;
                    chunk.addBlock(Block(BlockType::GRASS, blockPos));
                }
            }
        }
    }

    chunk.markBasePainted();
}

void Terrain::paintTerrainTop(Chunk &chunk)
{
    IntPos chunkPos = chunk.getChunkPos();
    if (!ChunkLoader::chunkLoaded(IntPos(chunkPos.x, chunkPos.y + 1, chunkPos.z)))
    {
        return;
    }

    const Chunk topChunk = ChunkLoader::getChunk(IntPos(chunkPos.x, chunkPos.y + 1, chunkPos.z));

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            if (chunk.getBlock(IntPos(x, CHUNK_SIZE - 1, z)).blockType == BlockType::DIRT
            && topChunk.getBlock(IntPos(x, 0, z)).blockType == BlockType::EMPTY)
            {
                IntPos blockPos = chunk.getBlock(IntPos(x, CHUNK_SIZE - 1, z)).pos;
                chunk.addBlock(Block(BlockType::GRASS, blockPos));
            }
        }
    }

    chunk.markTopPainted();
}
