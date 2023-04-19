#ifndef CURVEMAP_H
#define CURVEMAP_H
#include <vector>
#include "vec2.h"

class CurveMap
{
    private:
        std::vector<Vec2> points = {};
        bool isError = false;

    public:
        CurveMap();
        void setPoints(std::vector<Vec2> points);
        float map(float x);
        bool error();
};

#endif