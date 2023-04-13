#include "settingsMenu.h"
#include "pauseMenu.h"
#include "../globals.h"
#include "../input.h"

Button SettingsMenu::backButton;
ToggleButton SettingsMenu::toggleWireframeModeButton;
Textbox SettingsMenu::toggleWireframeModeTextbox;
bool SettingsMenu::shown = false;

void SettingsMenu::init()
{
    backButton.init(50, 50, 150, 30, uiTextColor, "back", uiColor);

    toggleWireframeModeTextbox.init(57, 105, "wireframe mode", uiTextColor, false, true);
    toggleWireframeModeButton.init(250, 90, 50, 30, uiTextColor,
            {"off", "on"}, uiColor);
}

void SettingsMenu::update()
{
    backButton.update();
    toggleWireframeModeButton.update();

    if (backButton.leftMouseDown() || (Input::justPressed("ESC") && shown))
    {
        hide();
        PauseMenu::show();
    }

    if (toggleWireframeModeButton.leftMouseDown())
    {
        if (toggleWireframeModeButton.getText() == "on")
        {
            wireframeMode = true;
        }
        else
        {
            wireframeMode = false;
        }
    }
}

void SettingsMenu::draw()
{
    backButton.draw();
    toggleWireframeModeTextbox.draw();
    toggleWireframeModeButton.draw();
}

void SettingsMenu::show()
{
    backButton.show();
    toggleWireframeModeTextbox.show();
    toggleWireframeModeButton.show();

    toggleWireframeModeButton.setText(wireframeMode ? "on" : "off");

    shown = true;
}

void SettingsMenu::hide()
{
    backButton.hide();
    toggleWireframeModeTextbox.hide();
    toggleWireframeModeButton.hide();
    shown = false;
}