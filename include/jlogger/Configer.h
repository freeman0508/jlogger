#ifndef CONFIGER_H
#define CONFIGER_H
#include <map>
#include <iostream>
#include "SimpleIni.h"
class Configer : public CSimpleIniA
{
    public:
        Configer();
        ~Configer();
        void setMaxLogFileSize(long _MaxLogFileSize);
        long getMaxLogFileSize();
        void setMaxLogFileCount(long _axLogFileCount);
        int getMaxLogFileCount();
        std::string getLogFileName(const std::string &_LogID);
        void addLogID(const std::string &_LogID, const std::string &_LogFilePath);
        const std::map<std::string, std::string> &getLogIDMap();
        void dumpConfiger();
    private:
        std::map<std::string, std::string> mLogIDMap;
        long mMaxLogFileSize;
        int mMaxLogFileCount;
};
#endif
