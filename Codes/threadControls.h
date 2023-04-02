#ifndef THREADCONTROLS_H
#define THREADCONTROLS_H

#include <vector>
#include <functional>
#include <thread>
#include <mutex>

class ThreadControls
{
    private:
        static const int SAFE_THREAD;

        static std::vector<std::thread> threads;
        static std::vector<std::vector<int>> groups;
        static std::vector<bool> running;
        static std::vector<bool> cycleFinished;
        static int currentRunningGroup;

        static int mainThreadsGroup;
        static bool mainThreadCycleFinished;

        static std::mutex mutex;

        static void addThread(int group, std::function<void()> threadFunction);

        static void updateGroupStatus();
        static void markThreadCycleFinished(int threadIndex);



        static std::vector<std::function<void()>> safeThreadFunctions;

        static void addSafeThreadFunction(std::function<void()> safeThreadFunction);
        static void runSafeThread();

    public:
        static void init();
        static void lockMainThread();
        static void markMainThreadCycleFinished();
        static void release();
};

#endif