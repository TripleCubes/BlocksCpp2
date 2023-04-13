#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "../UI/button.h"

class PauseMenu
{
    private:
        static Button backToGameButton;
        static Button settingsButton;
        static Button saveAndQuitButton;
        static bool shown;
        static bool shownFirstFrame;

    public:
        static void init();
        static void update();
        static void draw();
        static void show(bool fromGame = false);
        static void hide();
        static bool getShown();
};

#endif