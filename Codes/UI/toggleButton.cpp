#include "toggleButton.h"
#include <stdio.h>

void ToggleButton::init(float x, float y, float w, float h, Color color, 
                                std::vector<std::string> textList, Color textColor, bool textCentered)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->color = color;
    this->textList = std::vector<std::string>(textList);
    this->textColor = textColor;
    this->textCentered = textCentered;

    this->text = textList[0];
}

void ToggleButton::update()
{
    Button::update();

    if (Button::leftMouseDown())
    {
        textIndex++;
        if (textIndex == textList.size())
        {
            textIndex = 0;
        }
        text = textList[textIndex];
    }

    if (onUpdate)
    {
        onUpdate(this);
    }

    if (onMouseOn && mouseOn())
    {
        onMouseOn(this);
    }
    if (onMouseEnter && mouseEnter())
    {
        onMouseEnter(this);
    }
    if (onMouseLeave && mouseLeave())
    {
        onMouseLeave(this);
    }

    if (onLeftMouseDown && leftMouseDown())
    {
        onLeftMouseDown(this);
    }
    if (onLeftMouseUp && leftMouseUp())
    {
        onLeftMouseUp(this);
    }
    if (onLeftMouseHold && leftMouseHold())
    {
        onLeftMouseHold(this);
    }

    if (onRightMouseDown && rightMouseDown())
    {
        onRightMouseDown(this);
    }
    if (onRightMouseUp && rightMouseUp())
    {
        onRightMouseUp(this);
    }
    if (onRightMouseHold && rightMouseHold())
    {
        onRightMouseHold(this);
    }
}

void ToggleButton::show()
{
    if(onShow)
    {
        onShow(this);
    }
    Button::show();
}

void ToggleButton::hide()
{
    if(onHide)
    {
        onHide(this);
    }
    Button::hide();
}

std::string ToggleButton::getText()
{
    return text;
}

void ToggleButton::setText(std::string text)
{
    bool textFound = false;

    for (int i = 0; i < textList.size(); i++)
    {
        if (text == textList[i])
        {
            this->text = text;
            textIndex = i;
            textFound = true;
        }
    }

    if (!textFound)
    {
        printf("%s is not in toggleButton's textList\n", text.c_str());
    }
}