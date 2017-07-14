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
#include "Configer.h"


#define VERSION 1.1.0

#define LOGGER Logger::Instance()
//define some global interface
#define INIT_LOGGER LOGGER
#define LOAD_CONFIG(x) LOGGER.loadConfigFile(x)
#define SET_MAX_LOG_FILE_SIZE(x) Logger::Instance().setMaxLogFileSize(x)
#define SET_MAX_LOG_FILE_COUNT(x) Logger::Instance().setMaxLogFileCount(x)
#define DUMP_CONFIGER Logger::Instance().dumpConfiger()
#define ADD_LOGGER_ID(x, y) Logger::Instance().addLogID(x,y)

//define some AMCRO to make usage more simply
//#define ADD_LOGGER(x,y,z) Logger::Instance().openLogFile(x,y,z)
//#define INFO_LOGGER LOGGER << INFO 
#define INFO_LOGGER LOGGER << "ROOT" << SETID << INFO
#define INFO_LOGGER_ID(x) LOGGER << x << SETID << INFO

#define WARNNING_LOGGER LOGGER << "ROOT" << SETID <<  WARNNING
#define WARNNING_LOGGER_ID(x) LOGGER << x << SETID <<  WARNNING

#define ERROR_LOGGER LOGGER << "ROOT" << SETID << ERROR
#define ERROR_LOGGER_ID(x) LOGGER << x << SETID << ERROR

#define CRITICAL_LOGGER LOGGER << "ROOT" << SETID << CRITICAL 
#define CRITICAL_LOGGER_ID(x) LOGGER << x << SETID << CRITICAL 

#define DEBUG_LOGGER LOGGER<< "ROOT" << SETID << DEBUG
#define DEBUG_LOGGER_ID(x) LOGGER<< x << SETID << DEBUG

#define NOLEVEL_LOGGER LOGGER<< "ROOT" << SETID << NOLEVEL
#define NOLEVEL_LOGGER_ID(x) LOGGER<< x << SETID << NOLEVEL


//define const 
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
    template <class T> Logger& operator << (const T &_Message)
    {
        unsigned long tmpThreadID = pthread_self();
        if(0 == mLSList.count(tmpThreadID))
        {
            mLSList[tmpThreadID]= new LogStream();
        }
        *mLSList[tmpThreadID] << _Message;
        return *this;
    }
    /*
    Logger& operator << (const char *str);
    Logger& operator << (const std::string &_Message);
    Logger& operator<< (int number);
    Logger& operator<< (long number);
    Logger& operator<< (double number);
    */
    //Logger& operator<< (bool ok);
    //overload operator << to support: LOGGER << pfunc << str2 << END_LOGGER, pfunc a function pointer to do somethigs
    Logger& operator <<(Logger& (*pfunc)(Logger&));
    std::string getLevelMarker(LEVEL level);
	std::string getTime();
    void addLogID(const std::string &_LogID, const std::string &_LogFileName);
    void loadConfigFile(const std::string &_ConfigFilePath);
    void setMaxLogFileSize(long _Size);
    void setMaxLogFileCount(int _Count);
    void dumpConfiger();
    void makeLogHeader(LEVEL _Level);
    std::string getString();

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
    std::map<unsigned long, LogStream *> mLSList;
    Wqueue mQueue;
    ConsumeThread *mConsumeThread;
    Configer mConfiger;
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
