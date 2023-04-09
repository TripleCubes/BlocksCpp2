#include "threadControls.h"
#include "globals.h"
#include "ChunkLoader/chunkLoader.h"
#include <glm/glm.hpp>
#include <stdio.h>

const int ThreadControls::SAFE_THREAD = -1;

std::vector<std::thread> ThreadControls::threads;
std::vector<std::vector<int>> ThreadControls::groups;
std::vector<bool> ThreadControls::running;
std::vector<bool> ThreadControls::cycleFinished;
int ThreadControls::currentRunningGroup = 0;

int ThreadControls::mainThreadsGroup = 0;
bool ThreadControls::mainThreadCycleFinished = false;

std::mutex ThreadControls::mutex;

void ThreadControls::init()
{
    for (int i = 0; i <= 0; i++)
    {
        groups.push_back(std::vector<int>());
    }
    
    addThread(0, ChunkLoader::chunkLoadThreadFunction);
    addThread(0, ChunkLoader::updateSurfaceDataThreadFunction);

    addThread(SAFE_THREAD, ChunkLoader::updateMeshesThreadFunction);
    addThread(SAFE_THREAD, ChunkLoader::chunkUnloadThreadFunction);
}

void ThreadControls::addThread(int group, std::function<void()> threadFunction)
{
    if (group == SAFE_THREAD)
    {
        addSafeThreadFunction(threadFunction);
        return;
    }

    if (group >= groups.size())
    {
        printf("Group doesnt exist\n");
        return;
    }

    int currentThreadIndex = threads.size();
    groups[group].push_back(currentThreadIndex);
    running.push_back(true);
    cycleFinished.push_back(false);

    threads.push_back(std::thread([threadFunction, currentThreadIndex]()
    {
        while (running[currentThreadIndex])
        {
            bool runThread;
            do
            {
                std::lock_guard<std::mutex> guard(mutex);
                runThread = !cycleFinished[currentThreadIndex] 
                            && std::find(groups[currentRunningGroup].begin(), 
                                        groups[currentRunningGroup].end(), 
                                        currentThreadIndex) != groups[currentRunningGroup].end();
                if (!running[currentThreadIndex])
                {
                    runThread = true;
                }
            }
            while (!runThread);

            threadFunction();

            markThreadCycleFinished(currentThreadIndex);
        }
    }));
}

void ThreadControls::updateGroupStatus()
{
    bool allThreadsInRunningGroupFinished = true;
    for (int i = 0; i < groups[currentRunningGroup].size(); i++)
    {
        int threadIndex = groups[currentRunningGroup][i];
        if (!cycleFinished[threadIndex])
        {
            allThreadsInRunningGroupFinished = false;
        }
    }
    if (currentRunningGroup == mainThreadsGroup && !mainThreadCycleFinished)
    {
        allThreadsInRunningGroupFinished = false;
    }

    if (allThreadsInRunningGroupFinished)
    {
        for (int i = 0; i < groups[currentRunningGroup].size(); i++)
        {
            int threadIndex = groups[currentRunningGroup][i];
            cycleFinished[threadIndex] = false;
        }
        if (currentRunningGroup == mainThreadsGroup)
        {
            mainThreadCycleFinished = false;
        }

        currentRunningGroup++;
        if (currentRunningGroup >= groups.size())
        {
            currentRunningGroup = SAFE_THREAD;
        }
    }
}

void ThreadControls::markThreadCycleFinished(int threadIndex)
{
    std::lock_guard<std::mutex> guard(mutex);
    cycleFinished[threadIndex] = true;
    updateGroupStatus();
}

void ThreadControls::lockMainThread()
{
    bool runMainThread;
    do
    {
        std::lock_guard<std::mutex> guard(mutex);
        runMainThread = !mainThreadCycleFinished && currentRunningGroup == mainThreadsGroup;

        if (glfwWindowShouldClose(glfwWindow))
        {
            runMainThread = true;
        }

        if (currentRunningGroup == SAFE_THREAD)
        {
            runSafeThread();
            currentRunningGroup++;
        }
    }
    while (!runMainThread);
}

void ThreadControls::markMainThreadCycleFinished()
{
    std::lock_guard<std::mutex> guard(mutex);
    mainThreadCycleFinished = true;
    updateGroupStatus();
}

std::vector<std::function<void()>> ThreadControls::safeThreadFunctions;

void ThreadControls::addSafeThreadFunction(std::function<void()> safeThreadFunction)
{
    std::lock_guard<std::mutex> guard(mutex);
    safeThreadFunctions.push_back(safeThreadFunction);
}

void ThreadControls::runSafeThread()
{
    for (int i = 0; i < safeThreadFunctions.size(); i++)
    {
        safeThreadFunctions[i]();
    }
}

void ThreadControls::release()
{
    for (int i = 0; i < running.size(); i++)
    {
        running[i] = false;
    }

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}