#ifndef VEC2_H
#define VEC2_H

#include <string>

struct Vec2
{
    float x = 0;
    float y = 0;
    Vec2(float x, float y);
    Vec2 operator + (Vec2 vec);
    Vec2 operator - (Vec2 vec);
    Vec2 operator * (float num);
    Vec2 operator / (float num);
    Vec2 normalize();
    float length();
    float getYFromX(Vec2 vec, float x);

    std::string toString(bool endline = true);
};

#endif