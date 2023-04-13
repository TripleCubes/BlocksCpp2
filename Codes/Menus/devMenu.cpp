#include "devMenu.h"
#include "../globals.h"
#include "../input.h"

Button DevMenu::backToGameButton;
CurveGraph DevMenu::testGraph;

bool DevMenu::shown = false;
bool DevMenu::shownFirstFrame = false;

void DevMenu::init()
{
    backToGameButton.init(50, 50, 150, 30, uiColor, "back to game", uiTextColor);
    testGraph.init(currentWindowWidth - 350, currentWindowHeight - 350, 300, 300, uiColor, uiTextColor);
}

void DevMenu::update()
{
    backToGameButton.update();
    testGraph.update();

    if (backToGameButton.leftMouseDown() || ((Input::justPressed("ESC") || Input::justPressed("P")) && shown && !shownFirstFrame))
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
    testGraph.draw();
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
    testGraph.show();

    shown = true;
    shownFirstFrame = true;
    openingMenuGroup = DEV;
}

void DevMenu::hide()
{
    backToGameButton.hide();
    testGraph.hide();

    shown = false;
}