#include "ConsumeThread.h"
#include <unistd.h>
#include <sstream>
#include "Logger.h"

ConsumeThread::ConsumeThread(Wqueue & queue, Configer &_Configer):m_queue(queue), mConfiger(_Configer)
{
    mLogIDLogFileStream.clear();
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
        std::string tmpTotalMessage = m_queue.remove();
        std::string tmpLogID = getLogID(tmpTotalMessage);
        std::string tmpLogMessage = getLogMessage(tmpTotalMessage);
        if(""==tmpLogID)
        {
            //sleep(1);
            continue;
        }
        if(0==mLogIDLogFileStream.count(tmpLogID))
        {
            std::cout <<  tmpLogID << " is not exists!" << std::endl;
            std::cout << tmpTotalMessage << std::endl;
            openLogFile(tmpLogID);
        }
        if(isMaxSize(tmpLogID))
        {
            rollLogFile(tmpLogID);
        }
        *mLogIDLogFileStream[tmpLogID] << tmpLogMessage;
        mLogIDLogFileStream[tmpLogID]->flush();
    }
    mIsRunning = false;
    return NULL;
}
std::string ConsumeThread::getLogMessage(const std::string &_Message)
{
    std::string::size_type fmLocation = _Message.find(">>>");
    if(fmLocation != std::string::npos)
    {
        return _Message.substr(fmLocation+3);
    }
    return "";
}
std::string ConsumeThread::getLogID(const std::string &_Message)
{
    std::string::size_type fmLocation = _Message.find(">>>");
    if(fmLocation != std::string::npos)
    {
        return _Message.substr(0, fmLocation);
    }
    return "";
}
ConsumeThread::~ConsumeThread()
{
    std::cout << "ConsumeThread::~ConsumeThread" << std::endl;
    while(mIsRunning)
    {
        if(0 == m_queue.size())
        {
            std::cout << "ConsumeThread::~ConsumeThread 0" << std::endl;
            isRunnable = false;
        }
    }
    std::cout << "ConsumeThread::~ConsumeThread finished" << std::endl;
    mLogFileStream.close();
    std::map<std::string, std::string> tmpMap = mConfiger.getLogIDMap();
    std::map<std::string, std::string>::iterator fmIt = tmpMap.begin();
    for(; fmIt != tmpMap.end(); fmIt++)
    {
        std::string tmpLogID = fmIt->first;
        std::string tmpLogFileName = fmIt->second;
        std::cout << "ConsumeThread::~ConsumeThread" << tmpLogID << "=>" << tmpLogFileName << std::endl;
        std::fstream *fmFS = NULL;
        if(0<mLogIDLogFileStream.count(tmpLogID))
        {
            fmFS = mLogIDLogFileStream[tmpLogID];
            fmFS->close();
            delete fmFS; 
            mLogIDLogFileStream.erase(tmpLogID);
        }
    }
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
    //INFO_LOGGER << "ConsumeThread::rollLogFile" << END_LOGGER;
    std::fstream *fmFS = mLogIDLogFileStream[_LogID];
    std::stringstream fmSS;
    fmSS.str("");
    fmFS->close();
    //rename Log File
    std::string fmLogFileName = mConfiger.getLogFileName(_LogID);
    if(mLogNumber<mMaxLogFileCount)
    {
        //std::cout << mLogNumber << "1 ConsumeThread::rollLogFile" << std::endl;
        for(int i=mLogNumber; i>=0; i--)
        {
        //std::cout << mLogNumber << "2 ConsumeThread::rollLogFile" << std::endl;
            if(0==i)
            {
        //std::cout << mLogNumber << "3 ConsumeThread::rollLogFile" << std::endl;
                fmSS << fmLogFileName;
            }
            else
            {
        //std::cout << mLogNumber << "4 ConsumeThread::rollLogFile" << std::endl;
                fmSS << fmLogFileName << "." << i;
            }
        //std::cout << mLogNumber << "5 ConsumeThread::rollLogFile" << std::endl;
            std::string tmpOldName = fmSS.str();
            fmSS.str("");
            fmSS << fmLogFileName << "." << i+1;
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
        std::string fmLogFileName = mConfiger.getLogFileName(_LogID);
        fmFS->open(fmLogFileName.c_str(),  std::fstream::out | std::fstream::app);
        if(!fmFS->is_open())
        {
            std::cout << "Open " << fmLogFileName << " failed." << std::endl;
        }
        mLogIDLogFileStream[_LogID]=fmFS;
}
void ConsumeThread::initLogFiles()
{
    std::cout << "ConsumeThread::initLogFiles" << std::endl;
    std::map<std::string, std::string> tmpMap = mConfiger.getLogIDMap();
    std::map<std::string, std::string>::iterator fmIt = tmpMap.begin();
    for(; fmIt != tmpMap.end(); fmIt++)
    {
        std::string tmpLogID = fmIt->first;
        std::string tmpLogFileName = fmIt->second;
        std::cout << "ConsumeThread::initLogFiles " << tmpLogID << "=>" << tmpLogFileName << std::endl;
        std::fstream *fmFS = NULL;
        if(0<mLogIDLogFileStream.count(tmpLogID))
        {
            fmFS = mLogIDLogFileStream[tmpLogID];
            fmFS->close();
            delete fmFS; 
            mLogIDLogFileStream.erase(tmpLogID);
        }
        fmFS=new std::fstream();
        fmFS->open(tmpLogFileName.c_str(),  std::fstream::out | std::fstream::app);
        if(!fmFS->is_open())
        {
            std::cout << "Open " << tmpLogFileName << " failed." << std::endl;
            exit(-1);
        }
        mLogIDLogFileStream[tmpLogID]=fmFS;
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
