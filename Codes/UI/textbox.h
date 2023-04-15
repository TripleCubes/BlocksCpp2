#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "uiElement.h"
#include "../Types/color.h"
#include <string>

class Textbox: public UIElement
{
    private:
        std::string text = "";
        float x = 0;
        float y = 0;
        float w = 0;
        float h = 0;
        Color textColor = Color(1.0, 1.0, 1.0, 1.0);
        bool centeredX = false;
        bool centeredY = false;
        bool shown = false;

    public:
        void init(float x, float y, std::string text, 
                    Color textColor = Color(1.0, 1.0, 1.0, 1.0), 
                    bool centeredX = false, bool centeredY = false);
        void draw() override;
        void show() override;
        void hide() override;

        void setText(std::string text);
};

#endif