#include "color.h"

Color::Color(float r, float g, float b, float a): r{r}, g{g}, b{b}, a{a} {}

Color Color::operator + (Color color) const
{
    return Color(r + color.r, g + color.g, b + color.b, a + color.a);
}

Color Color::operator - (Color color) const
{
    return Color(r - color.r, g - color.g, b - color.b, a - color.a);
}