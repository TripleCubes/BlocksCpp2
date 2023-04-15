#include "uiValueUpdate.h"
#include "../intervalsAndWaits.h"

void UIValueUpdate::init(float updateDelay)
{
    this->updateDelay = updateDelay;
}

void UIValueUpdate::setValue(float value)
{
    currentValue = value;

    if (latestWaitId != -1)
    {
        stopWait(latestWaitId);
    }
    latestWaitId = wait(updateDelay, onValueUpdate);
}

float UIValueUpdate::getCurrentValue()
{
    return currentValue;
}
