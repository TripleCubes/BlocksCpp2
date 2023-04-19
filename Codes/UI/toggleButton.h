#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "button.h"
#include <vector>
#include <string>

class ToggleButton: public Button
{
    private:
        std::vector<std::string> textList = {};
        int textIndex = 0;

    public:        
        void init(float x, float y, float w, float h, Color color, 
                    std::vector<std::string> textList, Color textColor);
        void update() override;
        void show() override;
        void hide() override;
        std::string getText();
        void setText(std::string text);

        std::function<void(ToggleButton*)> onMouseEnter;
        std::function<void(ToggleButton*)> onMouseLeave;
        std::function<void(ToggleButton*)> onMouseOn;

        std::function<void(ToggleButton*)> onLeftMouseUp;
        std::function<void(ToggleButton*)> onLeftMouseDown;
        std::function<void(ToggleButton*)> onLeftMouseHold;

        std::function<void(ToggleButton*)> onRightMouseUp;
        std::function<void(ToggleButton*)> onRightMouseDown;
        std::function<void(ToggleButton*)> onRightMouseHold;

        std::function<void(ToggleButton*)> onUpdate;
        std::function<void(ToggleButton*)> onShow;
        std::function<void(ToggleButton*)> onHide;
};

#endif