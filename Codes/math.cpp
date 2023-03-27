#include "math.h"

int mod(int a, int b)
{
    return a % b >= 0 ? a % b : (a % b) + b;
}

bool isMax(float num1, float num2, float num3)
{
    if (num1 == 0)
    {
        return false;
    }
    if ((num1 < num2 && num2 != 0) || (num1 < num3 && num3 != 0))
    {
        return false;
    }
    return true;
}

bool isMin(float num1, float num2, float num3)
{
    if (num1 == 0)
    {
        return false;
    }
    if ((num1 > num2 && num2 != 0) || (num1 > num3 && num3 != 0))
    {
        return false;
    }
    return true;
}