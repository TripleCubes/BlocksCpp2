#include "intervalsAndWaits.h"
#include "gametime.h"
#include <vector>
#include <stdio.h>

int latestIntervalId = 0;

struct Interval
{
    std::function<void()> intervalFunction;
    float delay = 0;
    int looped = 0;
    int numberOfLoops = 0;
    float createdWhen = 0;
    bool finished = false;
    int id = -1;
};
std::vector<Interval> intervalList = {};

void updateIntervals()
{
    for (int i = 0; i < intervalList.size(); i++)
    {
        Interval &interval = intervalList[i];
        if (!interval.finished && Time::getCurrentTime() - interval.createdWhen > (interval.looped + 1) * interval.delay)
        {
            interval.looped++;
            interval.intervalFunction();
            if (interval.looped >= interval.numberOfLoops)
            {
                interval.finished = true;
            }
        }
    }
}

int setInterval(float delay, int numberOfLoops, std::function<void()> intervalFunction)
{
    int index = -1;
    for (int i = 0; i < intervalList.size(); i++)
    {
        if (intervalList[i].finished)
        {
            index = i;
            break;
        }
    }

    Interval interval;
    interval.createdWhen = Time::getCurrentTime();
    interval.delay = delay;
    interval.numberOfLoops = numberOfLoops;
    interval.intervalFunction = intervalFunction;
    interval.id = latestIntervalId;
    latestIntervalId++;
    if (latestIntervalId > 1000000)
    {
        printf("interval id pass 1000000\n");
    }

    if (index != -1)
    {
        intervalList[index] = interval;
    }
    else
    {
        intervalList.push_back(interval);
    }

    return interval.id;
}

void stopInterval(int id)
{
    for (int i = 0; i < intervalList.size(); i++)
    {
        if (intervalList[i].id == id)
        {
            intervalList[i].finished = true;
        }
    }
}

int wait(float delay, std::function<void()> waitFunction)
{
    return setInterval(delay, 1, waitFunction);
}

void stopWait(int id)
{
    stopInterval(id);
}