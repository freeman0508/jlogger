#ifndef CONFIGER_H
#define CONFIGER_H
#include <map>
#include <iostream>
class Configer
{
    public:
        Configer();
        Configer(Configer &_Configer);
        ~Configer();
        void setMaxLogFileSize(long _MaxLogFileSize);
        long getMaxLogFileSize();
        void setMaxLogFileCount(long _axLogFileCount);
        int getMaxLogFileCount();
        std::string getLogFileName(const std::string &_LogID);
        void addLogID(const std::string &_LogID, const std::string &_LogFilePath);
        const std::map<std::string, std::string> &getLogIDMap();
    private:
        std::map<std::string, std::string> mLogIDMap;
        long mMaxLogFileSize;
        int mMaxLogFileCount;
};
#endif
