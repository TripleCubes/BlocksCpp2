#include "button.h"
#include "ui.h"
#include "../globals.h"

Button::Button(float x, float y, float w, float h, Color color): x{x}, y{y}, w{w}, h{h}, color{color} {}

void Button::update()
{
    if (!isHidden)
    {
        double mousePosX;
        double mousePosY;
        glfwGetCursorPos(glfwWindow, &mousePosX, &mousePosY);
        
        if (mousePosX >= x && mousePosX <= x + w && mousePosY >= y && mousePosY <= y + h)
        {
            if (!mouseIsOn)
            {
                mouseJustEnter = true;
            }
            else
            {
                mouseJustEnter = false;
            }
            mouseJustLeave = false;
            mouseIsOn = true;
        }
        else
        {
            if (mouseIsOn)
            {
                mouseJustLeave = true;
            }
            else
            {
                mouseJustLeave = false;
            }
            mouseJustEnter = false;
            mouseIsOn = false;
        }

        if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && mouseIsOn)
        {
            if (!leftMouseIsHold)
            {
                leftMouseJustDown = true;
            }
            else
            {
                leftMouseJustDown = false;
            }
            leftMouseJustUp = false;
            leftMouseIsHold = true;
        }
        else
        {
            if (leftMouseIsHold)
            {
                leftMouseJustUp = true;
            }
            else
            {
                leftMouseJustUp = false;
            }
            leftMouseJustDown = false;
            leftMouseIsHold = false;
        }

        if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            if (!rightMouseIsHold)
            {
                rightMouseJustDown = true;
            }
            else
            {
                rightMouseJustDown = false;
            }
            rightMouseJustUp = false;
            rightMouseIsHold = true;
        }
        else
        {
            if (rightMouseIsHold)
            {
                rightMouseJustUp = true;
            }
            else
            {
                rightMouseJustUp = false;
            }
            rightMouseJustDown = false;
            rightMouseIsHold = false;
        }
    }
    else
    {
        mouseJustEnter = false;
        mouseJustLeave = false;
        mouseIsOn = false;

        leftMouseJustDown = false;
        leftMouseJustUp = false;
        leftMouseIsHold = false;

        rightMouseJustDown = false;
        rightMouseJustUp = false;
        rightMouseIsHold = false;
    }
}

void Button::draw()
{
    if (!isHidden)
    {
        if (mouseIsOn)
        {
            UI::drawRectWH(x, y, w, h, color + Color(0.1, 0.1, 0.1, 0.0));
        }
        else
        {
            UI::drawRectWH(x, y, w, h, color);
        }
    }
}

bool Button::mouseEnter()
{
    return mouseJustEnter;
}

bool Button::mouseLeave()
{
    return mouseJustLeave;
}

bool Button::mouseOn()
{
    return mouseIsOn;
}

bool Button::leftMouseUp()
{
    return leftMouseJustUp;
}

bool Button::leftMouseDown()
{
    return leftMouseJustDown;
}

bool Button::leftMouseHold()
{
    return leftMouseIsHold;
}

bool Button::rightMouseUp()
{
    return rightMouseJustUp;
}

bool Button::rightMouseDown()
{
    return rightMouseJustDown;
}

bool Button::rightMouseHold()
{
    return rightMouseIsHold;
}

void Button::show()
{
    isHidden = false;
}

void Button::hide()
{
    isHidden = true;
}

float Button::getX()
{
    return x;
}

float Button::getY()
{
    return y;
}

void Button::setPos(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Button::setSize(float w, float h)
{
    this->w = w;
    this->h = h;
}
