#include "terrain.h"

FastNoiseLite Terrain::terrainNoise;
CurveMap Terrain::terrainCurveMap;

FastNoiseLite Terrain::bigMountainNoise;
FastNoiseLite Terrain::smallMountainNoise;

void Terrain::init()
{
    terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    terrainNoise.SetSeed(rand());
    terrainNoise.SetFractalOctaves(5);
    terrainNoise.SetFrequency(0.03);
    terrainNoise.SetFractalLacunarity(1.5);

    bigMountainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    bigMountainNoise.SetSeed(rand());
    bigMountainNoise.SetFractalOctaves(1);
    bigMountainNoise.SetFrequency(0.002);

    smallMountainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    smallMountainNoise.SetSeed(rand());
    smallMountainNoise.SetFractalOctaves(3);
    smallMountainNoise.SetFrequency(0.005);
}

Block Terrain::getBlock(IntPos blockPos)
{
    float bigMountainNoiseHeight = (bigMountainNoise.GetNoise((float)blockPos.x, (float)blockPos.z)+1)/2;
    float smallMountainNoiseHeight = (smallMountainNoise.GetNoise((float)blockPos.x, (float)blockPos.z)+1)/2;
    int combinedMountainNoiseHeight = round((bigMountainNoiseHeight * 0.3 + smallMountainNoiseHeight * 0.7) * 70);

    float terrainNoiseValue = (terrainNoise.GetNoise((float)blockPos.x, (float)blockPos.y, (float)blockPos.z)+1)/2;
    terrainNoiseValue = terrainCurveMap.map(terrainNoiseValue);
    int maxHeight = combinedMountainNoiseHeight + 50;
    int minHeight = -100;
    float terrainNoise_BlockFilterRange = 1-((float)blockPos.y-(float)minHeight)/((float)maxHeight-(float)minHeight);
    if (terrainNoiseValue < terrainNoise_BlockFilterRange)
    {
        return Block(TEST, blockPos);
    }

    return Block(EMPTY, IntPos(0, 0, 0));
}