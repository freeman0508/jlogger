#include "Configer.h"

Configer::Configer()
{
}
Configer::Configer(Configer &_Configer)
{
    *this = _Configer;
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
    mLogIDMap[_LogID]=_LogFilePath;
}
const std::map<std::string, std::string> &Configer::getLogIDMap()
{
    return mLogIDMap;
}
