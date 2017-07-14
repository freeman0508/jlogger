#include "Logger.h"
#include <sys/time.h>
#include <unistd.h>

// Global static pointer used to ensure a single instance of the class.
Logger* Logger::m_pInstance = NULL;  
pthread_mutex_t Logger::mMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Logger::mWriteMutex = PTHREAD_MUTEX_INITIALIZER;
Logger::GC Logger::gc;
/** This function is called to create an instance of the class. 
    Calling the constructor publicly is not allowed. The constructor 
    is private and is only called by this Instance function.
*/
Logger& Logger::Instance()
{
    if (NULL == m_pInstance)   // Only allow one instance of class to be generated.
    {
        pthread_mutex_lock(&mMutex);
        if (!m_pInstance)   // Only allow one instance of class to be generated.
        {
            m_pInstance = new Logger();
        }
        pthread_mutex_unlock(&mMutex);
    }
    return *m_pInstance;
}
Logger::Logger()
{
    std::cout << "Logger::Logger" << std::endl;
    mConfiger.addLogID("ROOT", "/tmp/jlogger.log");
    mConfiger.addLogID("TEST", "/tmp/jlogger_test.log");
    mConsumeThread = new ConsumeThread(mQueue, mConfiger);
    mConsumeThread->start();
}
void Logger::loadConfigFile(const std::string &_ConfigFilePath)
{
    mConfiger.LoadFile(_ConfigFilePath.c_str());
    int fmMaxLogFileCount = mConfiger.GetLongValue("Logger", "MaxLogFileCount", 10);
    long fmMaxLogFileSize = mConfiger.GetLongValue("Logger", "MaxLogFileSize", 100);
    std::cout << "Logger::Logger: fmMaxLogFileCount: " << fmMaxLogFileCount << std::endl;
    std::cout << "Logger::Logger: fmMaxLogFileSize: " << fmMaxLogFileSize << std::endl;
    mConfiger.setMaxLogFileSize(fmMaxLogFileSize);
    mConfiger.setMaxLogFileCount(fmMaxLogFileCount);
}
void Logger::setMaxLogFileSize(long _Size)
{
    mConfiger.setMaxLogFileSize(_Size);
}
void Logger::setMaxLogFileCount(int _Count)
{
    mConfiger.setMaxLogFileCount(_Count);
}
void Logger::dumpConfiger()
{
    mConfiger.dumpConfiger();
}
void Logger::addLogID(const std::string &_LogID, const std::string &_LogFileName)
{
    mConfiger.addLogID(_LogID, _LogFileName);
}
Logger& Logger::operator <<(Logger& (*pfunc)(Logger &_Logger))
{
    return pfunc(*this);
}
void Logger::writeLogToFile()
{
    unsigned long tmpThreadID = pthread_self();
    std::string tmpLoggerContect = mLSList[tmpThreadID]->toString()+"\n";
    mQueue.add(tmpLoggerContect);
    mLSList[tmpThreadID]->clear();
}
//convert: 2014-7-31 17:26:57>>[CRITICAL] Test critical logger!!! >> Test critical logger!!!
std::string Logger::getTime()
{
    std::stringstream date_stream;
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    struct tm *ti = localtime(&tv.tv_sec);;
    char timeBuf[50];
    strftime(timeBuf, sizeof(timeBuf)/sizeof(timeBuf[0]), "%Y-%m-%d/%H:%M:%S", ti);
    int musec = tv.tv_usec/1000;
    char millisecBuf[5];
    sprintf(millisecBuf, "%03d", musec);
    date_stream << timeBuf << "." << millisecBuf;
    return date_stream.str();
}
std::string Logger::getLevelMarker(LEVEL level)
{
    std::string tmp_marker;
    switch(level)
    {
	    case Logger::critical:
	        return MARKER_CRITICAL;
	    break;
	    case Logger::information:
	        return MARKER_INFORMATION;
	    break;
	    case Logger::warnning:
	        return MARKER_WARNNING;
	    break;
	    case Logger::error:
	        return MARKER_ERROR;
	    break;
	    case Logger::debug:
	        return MARKER_DEBUG;
	    case Logger::nolevel:
	        return MARKER_NOLEVEL;
	    break;
	    default:
	    break;
    }
    return tmp_marker;
}
void Logger::makeLogHeader(LEVEL _Level)
{
    *this << SEPARATOR;
    *this << getTime();
    *this << SEPARATOR;
    *this << getLevelMarker(_Level);
    *this << SEPARATOR;
}
std::string Logger::getString()
{
    return mLSList[pthread_self()]->toString();
}

//flags
Logger& INFO(Logger& _Logger)
{
    _Logger.makeLogHeader(Logger::information);
    return _Logger;
}
Logger& WARNNING(Logger& _Logger)
{
    _Logger.makeLogHeader(Logger::warnning);
    return _Logger;
}
Logger& ERROR(Logger& _Logger)
{
    _Logger.makeLogHeader(Logger::error);
    return _Logger;
}
Logger& CRITICAL(Logger& _Logger)
{
    _Logger.makeLogHeader(Logger::critical);
    return _Logger;
}
Logger& DEBUG(Logger& _Logger)
{
    _Logger.makeLogHeader(Logger::debug);
    return _Logger;
}
Logger& NOLEVEL(Logger& _Logger)
{
    _Logger.makeLogHeader(Logger::nolevel);
    return _Logger;
}
Logger& END_LOGGER(Logger& _Logger)
{
    _Logger.writeLogToFile();
    return _Logger;
}
Logger& SETID(Logger& _Logger)
{
    std::cout << "============SETID============" << std::endl;
    std::string tmpLogID = _Logger.getString();
    std::cout << tmpLogID  << std::endl;
    //mConfiger.addLogI(tmpLogID);
    return _Logger;
}
Logger::~Logger()
{
    std::map<unsigned long, LogStream *>::iterator it = mLSList.begin();
    for(; it != mLSList.end(); it++)
    {
        if(it->second != NULL)
        {
            std::cout << "*********delete thread id: " <<  it->first << std::endl;
            delete it->second;
            //mLSList.erase(it->first);
        }
    }
    if(mConsumeThread!= NULL)
    {
        delete mConsumeThread;
    }
    pthread_mutex_destroy(&mMutex);
    pthread_mutex_destroy(&mWriteMutex);
}
