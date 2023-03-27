#ifndef UI_H
#define UI_H

#include "mesh.h"
#include "shader.h"

const float CROSSHAIR_WIDTH = 2;
const float CROSSHAIR_HEIGHT = 14;

class UI
{
    private:
        static Mesh crosshairMesh;
        static Shader crosshairShader;

        static Mesh blockSelectionMesh;
        static Shader blockSelectionShader;

    public:
        static void init();
        static void drawCrosshair();
        static void drawSelectedBlock();
        static void release();
};

#endif