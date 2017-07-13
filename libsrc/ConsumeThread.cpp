#include "ConsumeThread.h"
#include <unistd.h>
#include <sstream>
#include "Logger.h"

ConsumeThread::ConsumeThread(Wqueue & queue, Configer &_Configer):m_queue(queue), mConfiger(_Configer)
{
    mLogFileName = mConfiger.getLogFileName("ROOT");
    mMaxLogFileCount = mConfiger.getMaxLogFileCount();
    mMaxLogFileSize = mConfiger.getMaxLogFileSize();
    isRunnable = true;
    mIsRunning = false;
    mLogNumber = 0;
    initLogFiles();
}
void* ConsumeThread::run() 
{
    mIsRunning = true;
    while(isRunnable)
    {
        std::string item = m_queue.remove();
        std::string tmpLogID = "ROOT";
        //std::cout << "ConsumeThread::" << item << std::endl;
        if(isMaxSize(tmpLogID))
        {
            rollLogFile(tmpLogID);
        }
        *mLogIDLogFileStream[tmpLogID] << item;
        //sleep(1);
    }
    mIsRunning = false;
    return NULL;
}
ConsumeThread::~ConsumeThread()
{
    std::cout << "ConsumeThread::~ConsumeThread" << std::endl;
    while(mIsRunning)
    {
        std::cout << "ConsumeThread::~ConsumeThread size:" << m_queue.size() << std::endl;
        if(0 == m_queue.size())
        {
            isRunnable = false;
        }
    }
    std::cout << "ConsumeThread::~ConsumeThread finished" << std::endl;
    mLogFileStream.close();
}
bool ConsumeThread::isRunning()
{
    return mIsRunning;
}
bool ConsumeThread::isMaxSize(const std::string &_LogID)
{
    //std::cout << "ConsumeThread::isMaxSize: " << _LogID << std::endl;
    long funFileSize = mLogIDLogFileStream[_LogID]->tellp();
    if(mMaxLogFileSize <= funFileSize)
    {
        return true;
    }
    return false;
}
void ConsumeThread::rollLogFile(const std::string &_LogID)
{
    std::cout << mLogNumber << "ConsumeThread::rollLogFile" << std::endl;
    INFO_LOGGER << "ConsumeThread::rollLogFile" << END_LOGGER;
    std::fstream *fmFS = mLogIDLogFileStream[_LogID];
    std::stringstream fmSS;
    fmSS.str("");
    fmFS->close();
    //rename Log File
    if(mLogNumber<mMaxLogFileCount)
    {
        //std::cout << mLogNumber << "1 ConsumeThread::rollLogFile" << std::endl;
        for(int i=mLogNumber; i>=0; i--)
        {
        //std::cout << mLogNumber << "2 ConsumeThread::rollLogFile" << std::endl;
            if(0==i)
            {
        //std::cout << mLogNumber << "3 ConsumeThread::rollLogFile" << std::endl;
                fmSS << mLogFileName;
            }
            else
            {
        //std::cout << mLogNumber << "4 ConsumeThread::rollLogFile" << std::endl;
                fmSS << mLogFileName << "." << i;
            }
        //std::cout << mLogNumber << "5 ConsumeThread::rollLogFile" << std::endl;
            std::string tmpOldName = fmSS.str();
            fmSS.str("");
            fmSS << mLogFileName << "." << i+1;
            std::string tmpNewName = fmSS.str();
            fmSS.str("");
        //std::cout << mLogNumber << "old: " << tmpOldName << " new: " << tmpNewName << std::endl;
            rename(tmpOldName.c_str(), tmpNewName.c_str());
        }
        //std::cout << mLogNumber << "6 ConsumeThread::rollLogFile" << std::endl;
        mLogNumber++;
    }
    else
    {
        mLogNumber = mMaxLogFileCount-1;
    }
    openLogFile(_LogID);
}
void ConsumeThread::openLogFile(const std::string &_LogID)
{

        std::fstream *fmFS = NULL;
        if(0<mLogIDLogFileStream.count(_LogID))
        {
            fmFS = mLogIDLogFileStream[_LogID];
            if(fmFS->is_open())
            {
                fmFS->close();
            }
            delete fmFS;
            mLogIDLogFileStream.erase(_LogID);
        }
        fmFS = new std::fstream();
        fmFS->open(mLogFileName.c_str(),  std::fstream::out | std::fstream::app);
        if(!fmFS->is_open())
        {
            std::cout << "Open " << mLogFileName << " failed." << std::endl;
        }
        mLogIDLogFileStream[_LogID]=fmFS;
}
void ConsumeThread::initLogFiles()
{
    std::map<std::string, std::string> tmpMap = mConfiger.getLogIDMap();
    std::map<std::string, std::string>::iterator fmIt = tmpMap.begin();
    for(; fmIt != tmpMap.end(); fmIt++)
    {
        std::cout << fmIt->first << "=>" << fmIt->second << std::endl;
        std::fstream *fmFS = NULL;
        if(0<mLogIDLogFileStream.count(fmIt->first))
        {
            fmFS = mLogIDLogFileStream[fmIt->first];
            fmFS->close();
            delete fmFS; 
            mLogIDLogFileStream.erase(fmIt->first);
        }
        fmFS=new std::fstream();
        fmFS->open(mLogFileName.c_str(),  std::fstream::out | std::fstream::app);
        if(!fmFS->is_open())
        {
            std::cout << "Open " << mLogFileName << " failed." << std::endl;
        }
        mLogIDLogFileStream[fmIt->first]=fmFS;
    }
}
/*
std::string Logger::removePrefix(const std::string &_content, LEVEL _level)
{
    std::string tmp_string = _content;
    std::stringstream tmp_ss;
    tmp_ss << getDelimiter() << getLevelMarker(_level);
    std::string::size_type tmp_pos = _content.find(tmp_ss.str());
    if(tmp_pos == std::string::npos)
    {
        tmp_string = _content;
    }
    else
    {
        tmp_string = _content.substr(tmp_pos+tmp_ss.str().length());
    }
    return tmp_string;
}
void Logger::resetLogger()
{
    ss.clear();
    ss.str("");
    level = Logger::information;
    setFstreamID("ROOT");
}
std::stringstream& Logger::getStringStream()
{
    return ss;
}
void Logger::cleanStringStream()
{
    ss.clear();
    ss.str("");
}
std::string& Logger::getDelimiter()
{
    return delimiter;
}
void Logger::setLevel(LEVEL _level)
{
    level = _level;
}
*/
