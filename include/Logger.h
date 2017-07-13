/*
Author: Jack Wang
Description:
Modify:
    Date: 20150906
    Content: 
        add internal class GC to implement RAII for Logger
        add ~Logger to close file when desconstruct
        add pthread_mutex_destroy to destroy mutex
    Date: 20151123
    Content:
        support date fomat:2015-11-23/13:34:54.495
        support multi log files handler, choose by SETID flag, default id: ROOT
*/
#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include "Wqueue.h"
#include "ConsumeThread.h"
#include "LogStream.h"

//define some AMCRO to make usage more simply
//#define ADD_LOGGER(x,y,z) Logger::Instance().openLogFile(x,y,z)
#define LOGGER Logger::Instance()
#define INFO_LOGGER LOGGER << INFO 
#define WARNNING_LOGGER LOGGER << WARNNING
#define ERROR_LOGGER LOGGER << ERROR
#define CRITICAL_LOGGER LOGGER << CRITICAL 
#define DEBUG_LOGGER LOGGER<< DEBUG
#define NOLEVEL_LOGGER LOGGER<< NOLEVEL

#define MARKER_INFORMATION "[INFORMATION]"
#define MARKER_WARNNING "[WARNNING]"
#define MARKER_ERROR "[ERROR]"
#define MARKER_CRITICAL "[CRITICAL]"
#define MARKER_DEBUG "[DEBUG]"
#define MARKER_NOLEVEL "[NOLEVEL]"

#define SEPARATOR ">>>"


class Logger{
public:
    //define Logger level
    enum LEVEL {information=0, warnning, error, critical, debug, nolevel};
    //single instance modle, static function to get ref of object 
    static Logger& Instance();
    //bool openLogFile(const std::string &logFile, const std::string &del=">>", const std::string &_ID="ROOT");
    void writeLogToFile();
    //overload operator << to support: LOGGER << str1 << str2 << END_LOGGER
    Logger& operator << (std::string _Message);
    /*
    Logger& operator << (char *str);
    Logger& operator<< (int number);
    Logger& operator<< (long number);
    Logger& operator<< (double number);
    */
    //Logger& operator<< (bool ok);
    //overload operator << to support: LOGGER << pfunc << str2 << END_LOGGER, pfunc a function pointer to do somethigs
    Logger& operator <<(Logger& (*pfunc)(Logger&));
    std::string getLevelMarker(LEVEL level);
	std::string getTime();
    void addLogFile(const std::string &_LogName, const std::string &_LogFileName);

private:
    //single instance modle, private empty and copy construct functions to forbid to instance this class 
    Logger();
    ~Logger();
    Logger(Logger const&){};
    Logger& operator=(Logger const&){ return *this; };  // assignment operator is private
    //static self
    static Logger* m_pInstance;
    static pthread_mutex_t mMutex;
    static pthread_mutex_t mWriteMutex;
    std::map<std::string, std::string> mLogFileList;
    std::map<unsigned long, LogStream *> mLSList;
    Wqueue mQueue;
    ConsumeThread *mConsumeThread;
    ///////

    //RAII
    class GC
    {
    public:
        ~GC()
        {
            if(m_pInstance != NULL)
            {
                delete m_pInstance;
                m_pInstance = NULL;
            }
        }
    };
    static GC gc;
};
//a global function to support LOGGER << pfunc << str2 << END_LOGGER. In these functions could do something before log message was writen into file
//flag
Logger& INFO(Logger& _Logger);
Logger& WARNNING(Logger& _Logger);
Logger& ERROR(Logger& _Logger);
Logger& CRITICAL(Logger& _Logger);
Logger& DEBUG(Logger& _Logger);
Logger& NOLEVEL(Logger& _Logger);
//end action
Logger& END_LOGGER(Logger& _Logger);
Logger& SETID(Logger& _Logger);

#endif
