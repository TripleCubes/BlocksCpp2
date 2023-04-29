#include "curveMap.h"
#include <stdio.h>

CurveMap::CurveMap()
{
    points.clear();
    points.push_back(Vec2(0, 0));
    points.push_back(Vec2(1, 1));
}

void CurveMap::setPoints(std::vector<Vec2> points)
{
    if (points.size() < 2)
    {
        printf("CurveMap: points.size() < 2\n");
        isError = true;
        return;
    }
    if (points[0].x != 0)
    {
        printf("CurveMap: points[0].x != 0\n");
        isError = true;
        return;
    }
    if (points[points.size()-1].x != 1)
    {
        printf("CurveMap: points[points.size()-1].x != 1\n");
        isError = true;
        return;
    }
    for (int i = 0; i < points.size(); i++)
    {
        if (i != 0 && points[i-1].x > points[i].x)
        {
            printf("CurveMap: points[i-1].x > points[i].x\n");
            isError = true;
            return;
        }
        if (points[i].x < 0 || points[i].x > 1 || points[i].y < 0 || points[i].y > 1)
        {
            printf("CurveMap: point out of range\n");
            isError = true;
            return;
        }
    }
    isError = false;
    this->points = points;
}

float CurveMap::map(float x) const
{
    if (x < 0 || x > 1)
    {
        printf("CurveMap: cant map x < 0 or x > 1\n");
        return 0;
    }
    if (x == 0)
    {
        return points[0].y;
    }
    else if (x == 1)
    {
        return points[points.size() - 1].y;
    }
    for (int i = 0; i < points.size(); i++)
    {
        if (points[i].x > x)
        {
            return points[i-1].getYFromX(points[i], x);
        }
    }
    return 0;
}

bool CurveMap::error() const
{
    return isError;
}