#include "textbox.h"
#include "../Graphics/text.h"

void Textbox::init(float x, float y, std::string text,
                    Color textColor, 
                    bool centeredX, bool centeredY)
{
    this->x = x;
    this->y = y;
    this->text = text;
    this->textColor = textColor;
    this->centeredX = centeredX;
    this->centeredY = centeredY;
}

void Textbox::draw()
{
    if (shown)
    {
        if (centeredX && centeredY)
        {
            Text::drawTextBoxCentered(x, y, text, textColor);
        }
        else if (centeredX)
        {
            Text::drawTextBoxCenteredHorizontally(x, y, text, textColor);
        }
        else if (centeredY)
        {
            Text::drawTextBoxCenteredVertically(x, y, text, textColor);
        }
        else
        {
            Text::drawTextBox(x, y, text, textColor);
        }
    }
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