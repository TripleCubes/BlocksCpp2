#ifndef BUTTON_H
#define BUTTON_H

#include "../Types/color.h"

class Button
{
    private:
        float x = 0;
        float y = 0;
        float w = 0;
        float h = 0;
        Color color;
        bool mouseJustEnter = false;
        bool mouseJustLeave = false;
        bool mouseIsOn = false;

        bool leftMouseJustDown = false;
        bool leftMouseJustUp = false;
        bool leftMouseIsHold = false;

        bool rightMouseJustDown = false;
        bool rightMouseJustUp = false;
        bool rightMouseIsHold = false;

        bool isHidden = false;

    public:
        Button(float x, float y, float w, float h, Color color);

        void update();
        void draw();

        bool mouseEnter();
        bool mouseLeave();
        bool mouseOn();

        bool leftMouseUp();
        bool leftMouseDown();
        bool leftMouseHold();

        bool rightMouseUp();
        bool rightMouseDown();
        bool rightMouseHold();

        void show();
        void hide();

        float getX();
        float getY();
        void setPos(float x, float y);
        void setSize(float w, float h);
};

#endif