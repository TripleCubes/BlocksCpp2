#ifndef MENU_H
#define MENU_H

#include "uiElement.h"
#include <vector>
#include <functional>
#include <memory>
#include "../globals.h"

class Menu
{
    private:
        MenuGroup menuGroup = MenuGroup::NONE;
        std::vector<std::unique_ptr<UIElement>> uiList = {};

        bool shown = false;
        bool shownFirstFrame = false;

    public:
        void init(MenuGroup menuGroup);
        void update();
        void draw();
        void show();
        void hide();

        bool getShown();
        bool getShownFirstFrame();

        void add(std::unique_ptr<UIElement> uiElement);

        std::function<void(Menu*)> onUpdate;
        std::function<void(Menu*)> onShow;
        std::function<void(Menu*)> onHide;
};

#endif