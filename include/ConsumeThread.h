#ifndef CONSUMERTHREAD_H
#define CONSUMERTHREAD_H
#include <fstream>
#include "Thread.h"
#include "Wqueue.h"

//10M
//#define LOGFILE_SIZE_LIMIT 10000000
#define LOGFILE_SIZE_LIMIT 1000000
#define LOGFILE_COUNT_LIMIT 10
class ConsumeThread : public Thread
{
  public:
    ConsumeThread(Wqueue & queue, const std::string &_LogFilePath);
    ~ConsumeThread();
    void* run();
    bool isRunning();
    bool isMaxSize();
  private:
    void rollLogFile();
    void openLogFile();
  private:
      Wqueue & m_queue;
      bool volatile isRunnable;
      bool volatile mIsRunning;
      std::fstream mLogFileStream;
      std::string mLogFileName;
      int mLogNumber;
};
#endif
