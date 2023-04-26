#ifndef TERRAIN_H
#define TERRAIN_H

#include <FastNoiseLite.h>
#include "../Types/intPos.h"
#include "../Types/curveMap.h"
#include "../UI/ui.h"
#include "block.h"

class Terrain
{
    private:
        static FastNoiseLite terrainShape_noise;
        static CurveMap terrain_curveMap;
        
        static FastNoiseLite bigMountains_noise;
        static CurveMap bigMountains_curveMap;

        static FastNoiseLite smallMountains_noise;
        static CurveMap smallMountains_curveMap;

        static int terrainHeight_max;

    public:
        static void init();
        static Block getBlock(IntPos blockPos);
        static void paintTerrainBase(Chunk &chunk);
        static void paintTerrainTop(Chunk &chunk);

        friend void UI::initMenus();
};

#endif