#include "terrain.h"

FastNoiseLite Terrain::terrain_noise;
CurveMap Terrain::terrain_curveMap;

FastNoiseLite Terrain::bigMountains_noise;
FastNoiseLite Terrain::smallMountains_noise;

void Terrain::init()
{
    terrain_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    terrain_noise.SetSeed(rand());
    terrain_noise.SetFractalOctaves(5);
    terrain_noise.SetFrequency(0.03);
    terrain_noise.SetFractalLacunarity(1.5);

    bigMountains_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    bigMountains_noise.SetSeed(rand());
    bigMountains_noise.SetFractalOctaves(1);
    bigMountains_noise.SetFrequency(0.002);

    smallMountains_noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    smallMountains_noise.SetSeed(rand());
    smallMountains_noise.SetFractalOctaves(3);
    smallMountains_noise.SetFrequency(0.005);
}

Block Terrain::getBlock(IntPos blockPos)
{
    float bigMountains_height = (bigMountains_noise.GetNoise((float)blockPos.x, (float)blockPos.z)+1)/2;
    float smallMountains_height = (smallMountains_noise.GetNoise((float)blockPos.x, (float)blockPos.z)+1)/2;
    int combinedMountainsHeight = round((bigMountains_height * 0.3 + smallMountains_height * 0.7) * 70);

    float terrain_noiseValue = (terrain_noise.GetNoise((float)blockPos.x, (float)blockPos.y, (float)blockPos.z)+1)/2;
    int maxMountainsHeight = combinedMountainsHeight + 50;
    int minMountainsHeight = -100;
    float terrain_noiseValueFilter = ((float)blockPos.y-(float)minMountainsHeight)/((float)maxMountainsHeight-(float)minMountainsHeight);
    if (terrain_noiseValueFilter <= 0)
    {
        return Block(TEST, blockPos);
    }
    else if (terrain_noiseValueFilter >= 1)
    {
        return Block(EMPTY, IntPos(0, 0, 0));
    }
    else
    {
        terrain_noiseValueFilter = terrain_curveMap.map(terrain_noiseValueFilter);
        if (terrain_noiseValue > terrain_noiseValueFilter)
        {
            return Block(TEST, blockPos);
        }
    }

    return Block(EMPTY, IntPos(0, 0, 0));
}