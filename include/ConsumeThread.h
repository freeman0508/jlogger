#ifndef CONSUMERTHREAD_H
#define CONSUMERTHREAD_H
#include <fstream>
#include "Thread.h"
#include "Wqueue.h"
#include "Configer.h"

class ConsumeThread : public Thread
{
  public:
    //ConsumeThread(Wqueue & queue, const std::string &_LogFilePath);
    ConsumeThread(Wqueue & queue, Configer &_Configer);
    ~ConsumeThread();
    void* run();
    bool isRunning();
    bool isMaxSize(const std::string &_LogID);
  private:
    void rollLogFile(const std::string &_LogID);
    void initLogFiles();
    void openLogFile(const std::string &_LogID);
  private:
      Wqueue & m_queue;
      bool volatile isRunnable;
      bool volatile mIsRunning;
      std::fstream mLogFileStream;
      std::map<std::string, std::fstream *> mLogIDLogFileStream;
      std::string mLogFileName;
      int mLogNumber;
      Configer &mConfiger;
      int mMaxLogFileCount;
      long mMaxLogFileSize;
};
#endif
