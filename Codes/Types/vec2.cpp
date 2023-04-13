#include "vec2.h"
#include <math.h>
#include <stdio.h>

Vec2::Vec2(float x, float y): x{x}, y{y} {}

Vec2 Vec2::operator + (Vec2 vec)
{
    return Vec2(x + vec.x, y + vec.y);
}

Vec2 Vec2::operator - (Vec2 vec)
{
    return Vec2(x - vec.x, y - vec.y);
}

Vec2 Vec2::operator * (float num)
{
    return Vec2(x * num, y * num);
}

Vec2 Vec2::operator / (float num)
{
    return Vec2(x / num, y / num);
}

Vec2 Vec2::normalize()
{
    return Vec2(x, y) / length();
}

float Vec2::length()
{
    return sqrt(x*x + y*y);
}

float Vec2::getYFromX(Vec2 vec, float x)
{
    if (x == this->x)
    {
        return this->y;
    }
    else if (x == vec.x)
    {
        return vec.y;
    }

    Vec2 differentsVec = vec - Vec2(this->x, this->y);
    if (differentsVec.x == 0)
    {
        printf("Vec2 getYFromX error: differentsVec.x == 0\n");
    }
    if (differentsVec.y == 0)
    {
        return this->y;
    }
    
    float m = (this->y - vec.y)/(this->x - vec.x);
    float a = this->y - m*this->x;
    return m*x + a;
}

std::string Vec2::toString(bool endline)
{
    return std::to_string(x) +  " " + std::to_string(y) + (endline ? "\n" : "");
}