#ifndef CURVEGRAPHCONTROLPOINT_H
#define CURVEGRAPHCONTROLPOINT_H

#include "button.h"
#include "../Types/color.h"
#include "../Types/rect.h"

class CurveGraphControlPoint
{
    private:
        Button point;
        Button curveControl1;
        Button curveControl2;
        Rect moveRegion = {0, 0, 0, 0};

        bool shown = false;

    public:
        void init(float x, float y, float moveRegionX, float moveRegionY, float moveRegionW, float moveRegionH,
                        Color pointColor, Color curveControlColor);
        void update();
        void draw();
        void show();
        void hide();

        Button& getPoint();
        Button& getCurveControl1();
        Button& getCurveControl2();
        Rect getMoveRegion();
};

#endif