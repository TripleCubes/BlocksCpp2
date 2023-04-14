#ifndef CURVEGRAPH_H
#define CURVEGRAPH_H

#include "../Graphics/text.h"
#include "../Types/color.h"
#include "../Types/vec2.h"
#include "ui.h"
#include "uiElement.h"
#include "button.h"
#include "curveGraphControlPoint.h"
#include <vector>

class CurveGraph: public UIElement
{
    private:
        Button addControlPointButton;

        float x = 0;
        float y = 0;
        float w = 0;
        float h = 0;
        Color color = Color(1.0, 1.0, 1.0, 1.0);
        Color textColor = Color(1.0, 1.0, 1.0, 1.0);

        std::vector<Vec2> calculatedPoints = {};
        std::vector<CurveGraphControlPoint> controlPoints = {};
        bool error = false;

        bool shown = false;

        void calculateMapPoints();

        void drawBezierCurves();

    public:
        void init(float x, float y, float w, float h, 
                            Color color = Color(1.0, 1.0, 1.0, 1.0), 
                            Color textColor = Color(1.0, 1.0, 1.0, 1.0));
        void update() override;
        void draw() override;
        void show() override;
        void hide() override;
};

#endif