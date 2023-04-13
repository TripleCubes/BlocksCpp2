#include "curveGraphControlPoint.h"
#include "../input.h"

void CurveGraphControlPoint::init(float x, float y, float moveRegionX, float moveRegionY, 
                                        float moveRegionW, float moveRegionH, 
                                        Color pointColor, Color curveControlColor)
{
    moveRegion.x = moveRegionX;
    moveRegion.y = moveRegionY;
    moveRegion.w = moveRegionW;
    moveRegion.h = moveRegionH;

    float pointSize = 10;
    point.init(x-pointSize/2 + moveRegionX, y-pointSize/2 + moveRegionY, pointSize, pointSize, pointColor);

    float curveControlSize = pointSize-4;
    curveControl1.init(x-curveControlSize/2 - 20 + moveRegionX, y-curveControlSize/2 + moveRegionY, 
                            curveControlSize, curveControlSize, curveControlColor);
    curveControl2.init(x-curveControlSize/2 + 20 + moveRegionX, y-curveControlSize/2 + moveRegionY, 
                            curveControlSize, curveControlSize, curveControlColor);
}

void CurveGraphControlPoint::update()
{
    point.update();
    curveControl1.update();
    curveControl2.update();
}

void CurveGraphControlPoint::draw()
{
    point.draw();
    curveControl1.draw();
    curveControl2.draw();
}

void CurveGraphControlPoint::show()
{
    point.show();
    curveControl1.show();
    curveControl2.show();
    shown = true;
}

void CurveGraphControlPoint::hide()
{
    point.hide();
    curveControl1.hide();
    curveControl2.hide();
    shown = false;
}

Button& CurveGraphControlPoint::getPoint()
{
    return point;
}

Button& CurveGraphControlPoint::getCurveControl1()
{
    return curveControl1;
}

Button& CurveGraphControlPoint::getCurveControl2()
{
    return curveControl2;
}

Rect CurveGraphControlPoint::getMoveRegion()
{
    return moveRegion;
}