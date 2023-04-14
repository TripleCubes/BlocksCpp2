#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "uiElement.h"
#include "../Types/color.h"
#include "../Types/vec2.h"
#include <functional>

class Button: public UIElement
{
    protected:
        float x = 0;
        float y = 0;
        float w = 0;
        float h = 0;
        Color color = Color(0.0, 0.0, 0.0, 1.0);
        bool mouseJustEnter = false;
        bool mouseJustLeave = false;
        bool mouseIsOn = false;

        bool leftMouseJustDown = false;
        bool leftMouseJustUp = false;
        bool leftMouseIsHold = false;
        bool leftMouseIsTrueHold = false;

        bool rightMouseJustDown = false;
        bool rightMouseJustUp = false;
        bool rightMouseIsHold = false;
        bool rightMouseIsTrueHold = false;

        bool isHidden = true;

        std::string text = "";
        Color textColor = Color(0, 0, 0, 0);
        bool textCentered = true;

    public:
        void init(float x, float y, float w, float h, Color color, 
                std::string text = "", Color textColor = Color(1.0, 1.0, 1.0, 1.0), 
                bool textCentered = false);

        virtual void update() override;
        virtual void draw() override;

        bool mouseEnter();
        bool mouseLeave();
        bool mouseOn();

        bool leftMouseUp();
        bool leftMouseDown();
        bool leftMouseHold();

        bool rightMouseUp();
        bool rightMouseDown();
        bool rightMouseHold();

        virtual void show() override;
        virtual void hide() override;

        Vec2 getPos();
        void setPos(float x, float y);
        void setPos(Vec2 pos);
        Vec2 getCenterPos();
        void setCenterPos(Vec2 pos);
        void setSize(float w, float h);
        void setSize(Vec2 size);

        std::function<void(Button*)> onMouseEnter;
        std::function<void(Button*)> onMouseLeave;
        std::function<void(Button*)> onMouseOn;

        std::function<void(Button*)> onLeftMouseUp;
        std::function<void(Button*)> onLeftMouseDown;
        std::function<void(Button*)> onLeftMouseHold;

        std::function<void(Button*)> onRightMouseUp;
        std::function<void(Button*)> onRightMouseDown;
        std::function<void(Button*)> onRightMouseHold;

        std::function<void(Button*)> onUpdate;
        std::function<void(Button*)> onShow;
        std::function<void(Button*)> onHide;
};

#endif