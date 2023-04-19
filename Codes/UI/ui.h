#ifndef UI_H
#define UI_H

#include "../Graphics/mesh.h"
#include "../Graphics/shader.h"
#include "../Types/color.h"
#include "menu.h"

const float CROSSHAIR_WIDTH = 2;
const float CROSSHAIR_HEIGHT = 14;

class UI
{
    private:
        static Mesh crosshairMesh;
        static Shader crosshairShader;

        static Mesh blockSelectionMesh;
        static Shader blockSelectionShader;

        static Mesh rectMesh;
        static Shader rectShader;

        static Menu menu_pause;
        static Menu menu_pause_settings;

        static Menu menu_dev;
        static Menu menu_dev_noises;
        static Menu menu_dev_noises_terrain;

    public:
        static void init();
        static void initMenus();
        static void update();

        static void drawMenus();

        static void drawCrosshair();
        static void drawSelectedBlock();

        static void drawRectPos(float x1, float y1, float x2, float y2, Color color);
        static void drawRectWH(float x, float y, float w, float h, Color color);

        static void release();
};

#endif