#ifndef COLOR_H
#define COLOR_H

struct Color
{
    float r = 0;
    float g = 0;
    float b = 0;
    float a = 0;
    Color(float r, float g, float b, float a);
    Color operator + (Color color);
};

#endif