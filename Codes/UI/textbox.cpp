#include "textbox.h"
#include "../Types/rect.h"
#include "../Graphics/text.h"

void Textbox::init(float x, float y, std::string text,
                    Color textColor, 
                    HorizontalAlignment horizontalAlignment,
                    VerticalAlignment verticalAlignment)
{
    this->x = x;
    this->y = y;
    this->text = text;
    this->textColor = textColor;
    this->horizontalAlignment = horizontalAlignment;
    this->verticalAlignment = verticalAlignment;
}

void Textbox::draw()
{
    if (!shown)
    {
        return;
    }

    if (horizontalAlignment == HorizontalAlignment::LEFT && verticalAlignment == VerticalAlignment::TOP)
    {
        Text::drawTextBox(x, y, text, textColor);
        return;
    }

    Rect size = Text::getTextBoxSize(text);
    float drawPosx = x;
    float drawPosy = y;

    if (horizontalAlignment == HorizontalAlignment::CENTER)
    {
        drawPosx = x - size.w/2;
    }
    else if (horizontalAlignment == HorizontalAlignment::RIGHT)
    {
        drawPosx = x - size.w;
    }

    if (verticalAlignment == VerticalAlignment::CENTER)
    {
        drawPosy = y - size.h/2;
    }
    else if (verticalAlignment == VerticalAlignment::BOTTOM)
    {
        drawPosy = y - size.h;
    }

    Text::drawTextBox(drawPosx, drawPosy, text, textColor);
}

void Textbox::show()
{
    shown = true;
}

void Textbox::hide()
{
    shown = false;
}

void Textbox::setText(std::string text)
{
    this->text = text;
}