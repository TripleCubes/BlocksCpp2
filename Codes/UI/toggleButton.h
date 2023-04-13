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
                    std::vector<std::string> textList, Color textColor, bool textCentered = false);
        void update();
        std::string getText();
        void setText(std::string text);
};

#endif