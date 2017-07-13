#include "ProduceThread.h"
#include <unistd.h>
#include "logger.h"
ProduceThread::ProduceThread() 
{
    isRunnable = true;
    mIsRunning = false;
}
void* ProduceThread::run() 
{
    mIsRunning = true;
        // Remove 1 item at a time and process it. Blocks if no items are 
        // available to process.
        while(isRunnable)
        {
            DEBUG_LOGGER << "=========produce test=========" << END_LOGGER;
            //sleep(1);
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
