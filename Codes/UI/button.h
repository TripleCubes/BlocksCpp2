#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "../Types/color.h"

class Button
{
    protected:
        float x = 0;
        float y = 0;
        float w = 0;
        float h = 0;
        Color color = Color(0.0, 0.0, 0.0, 1.0);
        bool mouseJustEnter = false;
        bool mouseJustLeave = false;
        bool mouseIsOn = false;

        bool leftMouseJustDown = false;
        bool leftMouseJustUp = false;
        bool leftMouseIsHold = false;

        bool rightMouseJustDown = false;
        bool rightMouseJustUp = false;
        bool rightMouseIsHold = false;

        bool isHidden = true;

        std::string text = "";
        Color textColor = Color(0, 0, 0, 0);
        bool textCentered = true;

    public:
        void init(float x, float y, float w, float h, Color color, 
                std::string text = "", Color textColor = Color(1.0, 1.0, 1.0, 1.0), 
                bool textCentered = false);

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