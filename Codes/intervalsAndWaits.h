#ifndef INTERVALSANDWAITS_H
#define INTERVALSANDWAITS_H

#include <functional>

void updateIntervals();

int setInterval(float delay, int numberOfLoops, std::function<void()> intervalFunction);
void stopInterval(int id);
int wait(float delay, std::function<void()> waitFunction);
void stopWait(int id);

#endif