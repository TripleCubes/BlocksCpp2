#include "curveGraph.h"
#include "../globals.h"
#include "../input.h"
#include "../Types/vec2.h"
#include "../Graphics/text.h"
#include <tgmath.h>
#include <functional>

void CurveGraph::init(float x, float y, float w, float h, Color color, Color textColor)
{
    this->x = x + 10;
    this->y = y + 10;
    this->w = w - 20;
    this->h = h - 20;
    this->color = color;
    this->textColor = textColor;

    addControlPointButton.init(this->x + this->w - 30, this->y + this->h - 30, 20, 20, color, "", textColor, true);

    calculateMapPoints();
}

void CurveGraph::update()
{
    bool controlPointsUpdated = false;

    addControlPointButton.update();
    if (addControlPointButton.leftMouseDown())
    {
        CurveGraphControlPoint controlPoint;
        controlPoint.init(w - 40, (float)40, x, y, w, h, color, Color(0.82, 0.82, 0.8, 1.0));
        controlPoint.show();
        controlPoints.push_back(controlPoint);
        controlPointsUpdated = true;
    }

    for (int i = 0; i < controlPoints.size(); i++)
    {
        controlPoints[i].update();
    }

    for (int i = 0; i < controlPoints.size(); i++)
    {
        CurveGraphControlPoint &controlPoint = controlPoints[i];

        if (controlPoint.getPoint().leftMouseHold())
        {
            Vec2 mousePos = Input::getMousePos();
            if (mousePos.x < controlPoint.getMoveRegion().x){
                mousePos.x = controlPoint.getMoveRegion().x;
            }
            if (mousePos.x > controlPoint.getMoveRegion().x + controlPoint.getMoveRegion().w)
            {
                mousePos.x = controlPoint.getMoveRegion().x + controlPoint.getMoveRegion().w;
            }
            if (mousePos.y < controlPoint.getMoveRegion().y){
                mousePos.y = controlPoint.getMoveRegion().y;
            }
            if (mousePos.y > controlPoint.getMoveRegion().y + controlPoint.getMoveRegion().h)
            {
                mousePos.y = controlPoint.getMoveRegion().y + controlPoint.getMoveRegion().h;
            }

            Vec2 control1ToPoint = controlPoint.getCurveControl1().getCenterPos() - controlPoint.getPoint().getCenterPos();
            controlPoint.getPoint().setCenterPos(mousePos);
            controlPoint.getCurveControl1().setCenterPos(controlPoint.getPoint().getCenterPos() + control1ToPoint);
            controlPoint.getCurveControl2().setCenterPos(controlPoint.getPoint().getCenterPos() - control1ToPoint);
            controlPointsUpdated = true;
            break;
        }

        if (controlPoint.getCurveControl1().leftMouseHold())
        {
            Vec2 mousePos = Input::getMousePos();
            if ((mousePos - controlPoint.getPoint().getCenterPos()).length() > 50)
            {
                mousePos = controlPoint.getPoint().getCenterPos() + (mousePos - controlPoint.getPoint().getCenterPos()).normalize() * 50;
            }
            if ((mousePos - controlPoint.getPoint().getCenterPos()).length() < 10)
            {
                mousePos = controlPoint.getPoint().getCenterPos() + (mousePos - controlPoint.getPoint().getCenterPos()).normalize() * 10;
            }

            controlPoint.getCurveControl1().setCenterPos(mousePos);
            Vec2 control1ToPoint = controlPoint.getCurveControl1().getCenterPos() - controlPoint.getPoint().getCenterPos();
            controlPoint.getCurveControl1().setCenterPos(controlPoint.getPoint().getCenterPos() + control1ToPoint);
            controlPoint.getCurveControl2().setCenterPos(controlPoint.getPoint().getCenterPos() - control1ToPoint);
            controlPointsUpdated = true;
            break;
        }

        if (controlPoint.getCurveControl2().leftMouseHold())
        {
            Vec2 mousePos = Input::getMousePos();
            if ((mousePos - controlPoint.getPoint().getCenterPos()).length() > 50)
            {
                mousePos = controlPoint.getPoint().getCenterPos() + (mousePos - controlPoint.getPoint().getCenterPos()).normalize() * 50;
            }
            if ((mousePos - controlPoint.getPoint().getCenterPos()).length() < 10)
            {
                mousePos = controlPoint.getPoint().getCenterPos() + (mousePos - controlPoint.getPoint().getCenterPos()).normalize() * 10;
            }

            controlPoint.getCurveControl2().setCenterPos(mousePos);
            Vec2 control2ToPoint = controlPoint.getCurveControl2().getCenterPos() - controlPoint.getPoint().getCenterPos();
            controlPoint.getCurveControl2().setCenterPos(controlPoint.getPoint().getCenterPos() + control2ToPoint);
            controlPoint.getCurveControl1().setCenterPos(controlPoint.getPoint().getCenterPos() - control2ToPoint);
            controlPointsUpdated = true;
            break;
        }

        if (controlPoint.getPoint().rightMouseDown())
        {
            controlPoints.erase(controlPoints.begin() + i);
            controlPointsUpdated = true;
            break;
        }
    }

    if (controlPointsUpdated)
    {
        calculateMapPoints();
    }
}

void CurveGraph::calculateMapPoints()
{
    calculatedPoints.clear();

    std::function<void(Vec2, Vec2, Vec2, Vec2)> calculatePointsOneBezier = [this](Vec2 point1, Vec2 point2, Vec2 point3, Vec2 point4) 
    {
        for (float t = 0; t <= 1; t += 0.05)
        {
            Vec2 drawPoint = point1*pow(1-t, 3) + point2*3*t*pow(1-t, 2) + point3*3*(1-t)*pow(t, 2) + point4*pow(t, 3);
            calculatedPoints.push_back(drawPoint);
        }
    };

    if (controlPoints.size() == 0)
    {
        Vec2 point1 = Vec2(x, h + y);
        Vec2 point2 = Vec2(x, h + y);
        Vec2 point3 = Vec2(w + x, y);
        Vec2 point4 = Vec2(w + x, y);
        calculatePointsOneBezier(point1, point2, point3, point4);
        return;
    }

    Vec2 point1 = Vec2(x, h + y);
    Vec2 point2 = Vec2(x, h + y);
    Vec2 point3 = controlPoints[0].getCurveControl1().getCenterPos();
    Vec2 point4 = controlPoints[0].getPoint().getCenterPos();
    calculatePointsOneBezier(point1, point2, point3, point4);

    for (int i = 0; i < controlPoints.size() - 1; i++)
    {
        Vec2 point1 = controlPoints[i].getPoint().getCenterPos();
        Vec2 point2 = controlPoints[i].getCurveControl2().getCenterPos();
        Vec2 point3 = controlPoints[i + 1].getCurveControl1().getCenterPos();
        Vec2 point4 = controlPoints[i + 1].getPoint().getCenterPos();
        calculatePointsOneBezier(point1, point2, point3, point4);
    }

    point1 = controlPoints[controlPoints.size() - 1].getPoint().getCenterPos();
    point2 = controlPoints[controlPoints.size() - 1].getCurveControl2().getCenterPos();
    point3 = Vec2(w + x, y);
    point4 = Vec2(w + x, y);
    calculatePointsOneBezier(point1, point2, point3, point4);

    error = false;
    for (int i = 0; i < calculatedPoints.size() - 1; i++)
    {
        if (calculatedPoints[i].x > calculatedPoints[i + 1].x)
        {
            error = true;
            break;
        }
    }
}

void CurveGraph::drawBezierCurves()
{
    for (int i = 0; i < calculatedPoints.size(); i++)
    {
        UI::drawRectPos(calculatedPoints[i].x - 1, calculatedPoints[i].y - 1, 
                        calculatedPoints[i].x + 1, calculatedPoints[i].y + 1, Color(0.82, 0.82, 0.8, 1.0));
    }
}

void CurveGraph::draw()
{   
    if (shown)
    {
        UI::drawRectWH(x - 10, y - 10, w + 20, h + 20, Color(0, 0, 0, 1));
        UI::drawRectWH(x - 3, y - 3, 3, h + 6, uiColor);
        UI::drawRectWH(x - 3, y+h, w + 6, 3, uiColor);
        if (error)
        {
            Text::drawTextBox(x + 20, y + 10, "error", Color(0.98, 0.53, 0.37, 1.0));
        }
        drawBezierCurves();
    }

    for (int i = 0; i < controlPoints.size(); i++)
    {
        controlPoints[i].draw();
    }

    addControlPointButton.draw();
    if (shown)
    {
        Vec2 pos = addControlPointButton.getPos();
        Text::drawTextBox(pos.x + 5, pos.y - 3, "+", textColor);
    }
}

void CurveGraph::show()
{
    addControlPointButton.show();

    for (int i = 0; i < controlPoints.size(); i++)
    {
        controlPoints[i].show();
    }

    shown = true;
}

void CurveGraph::hide()
{
    addControlPointButton.hide();

    for (int i = 0; i < controlPoints.size(); i++)
    {
        controlPoints[i].hide();
    }

    shown = false;
}