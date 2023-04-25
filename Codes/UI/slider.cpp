#include "slider.h"
#include "ui.h"
#include "../input.h"
#include "../Types/vec2.h"
#include <sstream>
#include <iomanip>

void Slider::init(float x, float y, float w, float min, float max, Color color, int decimalNumbers)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->min = min;
    this->max = max;
    this->color = color;
    this->decimalNumbers = decimalNumbers;
    currentValue = min;

    button.init(x-5, y-5, 10, 10, color);
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(decimalNumbers) << min;
    textbox.init(x+w+15, y-14, ss.str());
}

void Slider::update()
{
    button.update();

    if (button.leftMouseHold())
    {
        Vec2 mousePos = Input::getMousePos();
        if (mousePos.x < x)
        {
            mousePos.x = x;
        }
        else if (mousePos.x > x + w)
        {
            mousePos.x = x + w;
        }
        button.setCenterPos(Vec2(mousePos.x, button.getCenterPos().y));

        float currentValuePercentage = (mousePos.x - x) / w;
        currentValue = (max - min) * currentValuePercentage + min;

        std::stringstream ss;
        ss << std::fixed << std::setprecision(decimalNumbers) << currentValue;
        textbox.setText(ss.str());

        if (onLeftMouseHold)
        {
            onLeftMouseHold(this);
        }
    }

    if (button.leftMouseUp())
    {
        if (onLeftMouseUp)
        {
            onLeftMouseUp(this);
        }
    }
}

void Slider::draw()
{
    if (shown)
    {
        UI::drawRectWH(x-1, y-1, w, 2, Color(0.82, 0.82, 0.8, 1.0));
    }

    button.draw();
    textbox.draw();
}

void Slider::show()
{
    button.show();
    textbox.show();
    shown = true;
}

void Slider::hide()
{
    button.hide();
    textbox.hide();
    shown = false;
}

float Slider::getValue()
{
    return currentValue;
}