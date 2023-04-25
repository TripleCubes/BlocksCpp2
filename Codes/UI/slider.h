#ifndef SLIDER_H
#define SLIDER_H

#include "uiElement.h"
#include "button.h"
#include "textbox.h"
#include "../Types/color.h"
#include <functional>

class Slider: public UIElement
{
    private:
        Button button;
        Textbox textbox;

        float x = 0;
        float y = 0;
        float w = 0;
        float max = 0;
        float min = 0;
        float currentValue = 0;
        Color color = Color(1, 1, 1, 1);
        float decimalNumbers = 1;

        bool shown = false;

    public:
        void init(float x, float y, float w, float min, float max, 
                    Color color = Color(1, 1, 1, 1), int decimalNumbers = 1);
        void update() override;
        void draw() override;
        void show() override;
        void hide() override;

        float getValue();

        std::function<void(Slider*)> onLeftMouseUp;
        std::function<void(Slider*)> onLeftMouseHold;
};

#endif