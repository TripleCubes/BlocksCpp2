#include "devMenu.h"
#include "../globals.h"
#include "../input.h"

Button DevMenu::backToGameButton;
bool DevMenu::shown = false;
bool DevMenu::shownFirstFrame = false;

void DevMenu::init()
{
    backToGameButton.init(50, 50, 150, 30, uiTextColor, "back to game", uiColor);
}

void DevMenu::update()
{
    backToGameButton.update();

    if (backToGameButton.leftMouseDown() || (Input::justPressed("ESC") && shown && !shownFirstFrame))
    {
        mouseLock = true;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
        openingMenuGroup = NONE;

        hide();
    }

    shownFirstFrame = false;
}

void DevMenu::draw()
{
    backToGameButton.draw();
}

void DevMenu::show(bool fromGame)
{
    if (fromGame)
    {
        mouseLock = false;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
        glfwSetCursorPos(glfwWindow, currentWindowWidth/2, currentWindowHeight/2);
    }

    backToGameButton.show();

    shown = true;
    shownFirstFrame = true;
    openingMenuGroup = DEV;
}

void DevMenu::hide()
{
    backToGameButton.hide();

    shown = false;
}