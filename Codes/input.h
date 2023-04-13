#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <unordered_map>
#include "Types/vec2.h"

class Input
{
    private:
        struct Key 
        {
            int glfwKey = 0;
            bool pressed = false;
            bool justPressed = false;
        };
        static std::unordered_map<std::string, Key> keys;

        static void addKey(std::string key, int glfwKey);

    public:
        static void init();
        static void update();
        static bool pressed(std::string key);
        static bool justPressed(std::string key);

        static Vec2 getMousePos();
};

#endif