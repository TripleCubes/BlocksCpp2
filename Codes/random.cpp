#include "random.h"

std::mt19937 Random::rng;

void Random::init()
{
    std::random_device randomDevice;
    rng = std::mt19937(randomDevice());
}

int Random::getInt(int from, int to)
{
    std::uniform_int_distribution<> distribution(from, to);
    return distribution(rng);
}

float Random::getFloat(float from, float to)
{
    std::uniform_real_distribution<> distribution(from, to);
    return distribution(rng);
}

bool Random::dice(int chance, int of)
{
    int rnd = getInt(0, of - 1);
    if (rnd < chance)
    {
        return true;
    }
    return false;
}
