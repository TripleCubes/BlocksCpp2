#include "pauseMenu.h"
#include "settingsMenu.h"
#include "../globals.h"
#include "../input.h"

Button PauseMenu::backToGameButton;
Button PauseMenu::settingsButton;
Button PauseMenu::saveAndQuitButton;
bool PauseMenu::shown = false;
bool PauseMenu::shownFirstFrame = false;

void PauseMenu::init()
{
    backToGameButton.init(50, 50, 150, 30, uiColor, "back to game", uiTextColor);
    settingsButton.init(50, 90, 150, 30, uiColor, "settings", uiTextColor);
    saveAndQuitButton.init(50, 130, 150, 30, uiColor, "save and quit", uiTextColor);
}

void PauseMenu::update()
{
    backToGameButton.update();
    settingsButton.update();
    saveAndQuitButton.update();

    if (backToGameButton.leftMouseDown() || (Input::justPressed("ESC") && shown && !shownFirstFrame))
    {
        mouseLock = true;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
        openingMenuGroup = NONE;

        hide();
    }

    if (settingsButton.leftMouseDown())
    {
        hide();
        SettingsMenu::show();
    }

    shownFirstFrame = false;
}

void PauseMenu::draw()
{
    backToGameButton.draw();
    settingsButton.draw();
    saveAndQuitButton.draw();
}

void PauseMenu::show(bool fromGame)
{
    if (fromGame)
    {
        mouseLock = false;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
        glfwSetCursorPos(glfwWindow, currentWindowWidth/2, currentWindowHeight/2);
    }

    backToGameButton.show();
    settingsButton.show();
    saveAndQuitButton.show();
    shown = true;
    shownFirstFrame = true;

    openingMenuGroup = PAUSE;
}

void PauseMenu::hide()
{
    backToGameButton.hide();
    settingsButton.hide();
    saveAndQuitButton.hide();
    shown = false;
}

bool PauseMenu::getShown()
{
    return shown;
}