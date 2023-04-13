#ifndef DEVMENU_H
#define DEVMENU_H

#include "../UI/button.h"
#include "../UI/curveGraph.h"

class DevMenu
{
    private:
        static Button backToGameButton;
        static CurveGraph testGraph;

        static bool shown;
        static bool shownFirstFrame;

    public:
        static void init();
        static void update();
        static void draw();
        static void show(bool fromGame = false);
        static void hide();
};

#endif