#include "menu.h"

void Menu::init(MenuGroup menuGroup)
{
    this->menuGroup = menuGroup;
}

void Menu::update()
{
    for (int i = 0; i < uiList.size(); i++)
    {
        uiList[i]->update();
    }

    if (onUpdate)
    {
        onUpdate(this);
    }

    shownFirstFrame = false;
}

void Menu::draw()
{
    for (int i = 0; i < uiList.size(); i++)
    {
        uiList[i]->draw();
    }
}

void Menu::show()
{
    for (int i = 0; i < uiList.size(); i++)
    {
        uiList[i]->show();
    }

    if (onShow)
    {
        onShow(this);
    }

    shown = true;
    shownFirstFrame = true;
    openingMenuGroup = menuGroup;
}

void Menu::hide()
{
    for (int i = 0; i < uiList.size(); i++)
    {
        uiList[i]->hide();
    }

    if (onHide)
    {
        onHide(this);
    }

    shown = false;
    openingMenuGroup = NONE;
}

bool Menu::getShown()
{
    return shown;
}

bool Menu::getShownFirstFrame()
{
    return shownFirstFrame;
}

void Menu::add(std::unique_ptr<UIElement> uiElement)
{
    uiList.push_back(std::move(uiElement));
}