#ifndef UIVALUEUPDATE_H
#define UIVALUEUPDATE_H

#include <functional>

class UIValueUpdate
{
    private:
        float previousValue = 0;
        float currentValue = 0;
        float updateDelay = 0;
        float latestWaitId = -1;

    public:
        void init(float updateDelay);
        void update();
        void setValue(float value);
        float getCurrentValue();

        std::function<void()> onValueUpdate;
};

#endif