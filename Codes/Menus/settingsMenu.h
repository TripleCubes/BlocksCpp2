#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "../UI/button.h"
#include "../UI/toggleButton.h"
#include "../UI/textbox.h"

class SettingsMenu
{
    private:
        static Button backButton;
        static ToggleButton toggleWireframeModeButton;
        static Textbox toggleWireframeModeTextbox;
        static bool shown;

    public:
        static void init();
        static void update();
        static void draw();
        static void show();
        static void hide();
};

#endif