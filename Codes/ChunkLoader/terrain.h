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
        static FastNoiseLite terrainNoise;
        static CurveMap terrainCurveMap;
        
        static FastNoiseLite bigMountainNoise;
        static FastNoiseLite smallMountainNoise;

    public:
        static void init();
        static Block getBlock(IntPos blockPos);

        friend void UI::initMenus();
};

#endif