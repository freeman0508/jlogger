#include "Configer.h"

Configer::Configer()
{
    mMaxLogFileSize = 100000;
    mMaxLogFileCount = 10;
}
Configer::~Configer()
{}
 void Configer::setMaxLogFileSize(long _MaxLogFileSize)
{
    mMaxLogFileSize = _MaxLogFileSize;
}
long Configer::getMaxLogFileSize()
{
    return mMaxLogFileSize;
}
void Configer::setMaxLogFileCount(long _MaxLogFileCount)
{
    mMaxLogFileCount = _MaxLogFileCount;
}
int Configer::getMaxLogFileCount()
{
    return mMaxLogFileCount;
}
std::string Configer::getLogFileName(const std::string &_LogID)
{
    return mLogIDMap[_LogID];
}
void Configer::addLogID(const std::string &_LogID, const std::string &_LogFilePath)
{
    if(0==mLogIDMap.count(_LogID))
    {
        mLogIDMap[_LogID]=_LogFilePath;
    }
}
const std::map<std::string, std::string> &Configer::getLogIDMap()
{
    return mLogIDMap;
}
void Configer::dumpConfiger()
{
    std::cout << " mMaxLogFileCount: " << mMaxLogFileCount << std::endl;
    std::cout << " mMaxLogFileSize: " << mMaxLogFileSize << std::endl;
    std::cout << " mLogIDMap: " << std::endl;
    std::map<std::string, std::string>::iterator it = mLogIDMap.begin();
    for(;it!=mLogIDMap.end();it++)
    {
        std::cout << "  " << it->first << "=>" << it->second << std::endl;
    }
}
