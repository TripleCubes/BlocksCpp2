#ifndef UIELEMENT_H
#define UIELEMENT_H

class UIElement
{
    public:
        virtual void update();
        virtual void draw();
        virtual void show();
        virtual void hide();
};

#endif