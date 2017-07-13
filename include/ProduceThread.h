#ifndef PRODUCETHREAD_H
#define PRODUCETHREAD_H
#include "Thread.h"
#include "Wqueue.h"
class ProduceThread : public Thread
{
  public:
    ProduceThread(); 
    ~ProduceThread(); 
    void* run();
    void stop();
  private:
    bool volatile isRunnable;
    bool volatile mIsRunning;
};
#endif
