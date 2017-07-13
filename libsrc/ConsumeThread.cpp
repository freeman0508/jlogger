#include "ConsumeThread.h"
#include <unistd.h>
#include <sstream>

ConsumeThread::ConsumeThread(wqueue & queue, const std::string &_LogFilePath) : m_queue(queue), mLogFileName(_LogFilePath)
{
    isRunnable = true;
    mIsRunning = false;
    mLogNumber = 0;
    openLogFile();
}
void* ConsumeThread::run() 
{
    mIsRunning = true;
    while(isRunnable)
    {
        std::string item = m_queue.remove();
        std::cout << "ConsumeThread::" << item << std::endl;
        //rollLogFile();
        if(isMaxSize())
        {
            rollLogFile();
        }
        mLogFileStream << item;
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
bool ConsumeThread::isMaxSize()
{
    long funFileSize = mLogFileStream.tellp();
    if(LOGFILE_SIZE_LIMIT <= funFileSize)
    {
        return true;
    }
    return false;
}
void ConsumeThread::rollLogFile()
{
    std::stringstream fmSS;
    fmSS.str("");
    mLogFileStream.close();
    //rename Log File
    if(mLogNumber<LOGFILE_COUNT_LIMIT)
    {
        std::cout << mLogNumber << "1 ConsumeThread::~ConsumeThread" << std::endl;
        for(int i=mLogNumber; i>=0; i--)
        {
        std::cout << mLogNumber << "2 ConsumeThread::~ConsumeThread" << std::endl;
            if(0==i)
            {
        std::cout << mLogNumber << "3 ConsumeThread::~ConsumeThread" << std::endl;
                fmSS << mLogFileName;
            }
            else
            {
        std::cout << mLogNumber << "4 ConsumeThread::~ConsumeThread" << std::endl;
                fmSS << mLogFileName << "." << i;
            }
        std::cout << mLogNumber << "5 ConsumeThread::~ConsumeThread" << std::endl;
            std::string tmpOldName = fmSS.str();
            fmSS.str("");
            fmSS << mLogFileName << "." << i+1;
            std::string tmpNewName = fmSS.str();
            fmSS.str("");
        std::cout << mLogNumber << "old: " << tmpOldName << " new: " << tmpNewName << std::endl;
            rename(tmpOldName.c_str(), tmpNewName.c_str());
        }
        std::cout << mLogNumber << "6 ConsumeThread::~ConsumeThread" << std::endl;
        mLogNumber++;
    }
    else
    {
        mLogNumber = LOGFILE_COUNT_LIMIT-1;
    }
    openLogFile();
}
void ConsumeThread::openLogFile()
{
    mLogFileStream.open(mLogFileName.c_str(),  std::fstream::out | std::fstream::app);
    if(!mLogFileStream.is_open())
    {
        std::cout << "Open " << mLogFileName << " failed." << std::endl;
    }
}
/*
bool Logger::openLogFile(const std::string &_logFile, const std::string &_del, const std::string &_ID)
{
    if(mFSList.count(_ID)>0)
    {
        //std::cout << "fstream id: " << _ID << " is exists already." << std::endl;
		return false;
    }
    delimiter = _del;
    setLevelMarker();
    std::fstream *tmpFS = new std::fstream();
    tmpFS->open (_logFile.c_str(),  std::fstream::out | std::fstream::app);
    if(tmpFS->is_open())
    {
        //INFO_LOGGER << "======================================Logger Start======================================" << END_LOGGER;
        if(mFSList.empty())
        {
            mFSList["ROOT"] = tmpFS;
        }
        mFSList[_ID] = tmpFS;
        setFstreamID(_ID);
		return true;
    }
    else
    {
        //std::cout << "Logger is failed!" << std::endl;
		return false;
    }
}
Logger::LEVEL Logger::getLevel()
{
    return level;
}
void Logger::setLevelMarker(
        std::string _marker_information, 
        std::string _marker_warnning, 
        std::string _marker_error, 
        std::string _marker_critical, 
        std::string _marker_debug,
        std::string _marker_nolevel
    )
{
    marker_information = _marker_information;
    marker_warnning = _marker_warnning;
    marker_error = _marker_error;
    marker_critical = _marker_critical;
    marker_debug = _marker_debug;
    marker_nolevel = _marker_nolevel;
}
std::string Logger::getLevelMarker()
{
    return getLevelMarker(level);
}
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
