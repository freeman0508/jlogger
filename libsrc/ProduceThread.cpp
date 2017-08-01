#include "ProduceThread.h"
#include <unistd.h>
#include "Logger.h"
ProduceThread::ProduceThread() 
{
    std::cout << "    ProduceThread::ProduceThread" << std::endl;
    isRunnable = true;
    mIsRunning = false;
}
void* ProduceThread::run() 
{
    mIsRunning = true;
    while(isRunnable)
    {
        DEBUG_LOGGER << "=========produce test=========" << END_LOGGER;
        usleep(1000);
    }
    mIsRunning = false;
    return NULL;
}
void ProduceThread::stop()
{
    isRunnable = false;
    std::cout << "    ProduceThread::stop" << std::endl;
    while(mIsRunning)
    {
    }
    std::cout << "    ProduceThread::stop finished" << std::endl;
}
ProduceThread::~ProduceThread()
{
    std::cout << "    ProduceThread::~ProduceThread" << std::endl;
    if(isRunnable)
    {
        isRunnable = false;
    }
    while(mIsRunning)
    {
    }
    std::cout << "    ProduceThread::~ProduceThread finished" << std::endl;
}
