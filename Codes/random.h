#ifndef RANDOM_H
#define RANDOM_H
#include <random>

class Random
{
    private:
        static std::mt19937 rng;

    public:
        static void init();
        static int getInt(int from, int to);
        static float getFloat(float from, float to);
        static bool dice(int chance, int of);
};

#endif