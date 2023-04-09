#ifndef UI_H
#define UI_H

#include "../Graphics/mesh.h"
#include "../Graphics/shader.h"
#include "../Types/color.h"
#include "button.h"

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

        static Button testButton;

    public:
        static void init();
        static void update();

        static void hidePauseMenu();
        static void showPauseMenu();
        static void drawPauseMenu();

        static void drawCrosshair();
        static void drawSelectedBlock();

        static void drawRectPos(float x1, float y1, float x2, float y2, Color color);
        static void drawRectWH(float x, float y, float w, float h, Color color);

        static void release();
};

#endif