#include "input.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "globals.h"

std::unordered_map<std::string, Input::Key> Input::keys;

void Input::addKey(std::string keyString, int glfwKey)
{
    Key key = {glfwKey, false};
    keys.insert(std::make_pair(keyString, key));
}

void Input::init()
{
    addKey("ESC", GLFW_KEY_ESCAPE);
    addKey("A", GLFW_KEY_A);
    addKey("P", GLFW_KEY_P);
}

void Input::update()
{
    for (std::unordered_map<std::string, Input::Key>::iterator i = keys.begin(); i != keys.end(); i++)
    {
        if (glfwGetKey(glfwWindow, i->second.glfwKey) == GLFW_PRESS)
        {
            if (!i->second.pressed)
            {
                i->second.justPressed = true;
            }
            else
            {
                i->second.justPressed = false;
            }
            i->second.pressed = true;
        }
        else
        {
            i->second.pressed = false;
            i->second.justPressed = false;
        }
    }
}

bool Input::pressed(std::string key)
{
    if (keys.find(key) == keys.end())
    {
        printf("key %s isnt added\n", key.c_str());
        return false;
    }
    return keys[key].pressed;
}

bool Input::justPressed(std::string key)
{
    if (keys.find(key) == keys.end())
    {
        printf("key %s isnt added\n", key.c_str());
        return false;
    }
    return keys[key].justPressed;
}

Vec2 Input::getMousePos()
{
    double x = 0;
    double y = 0;
    glfwGetCursorPos(glfwWindow, &x, &y);
    return Vec2((float)x, (float)y);
}