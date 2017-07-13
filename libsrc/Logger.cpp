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
    if (!m_pInstance)   // Only allow one instance of class to be generated.
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
    mConsumeThread = new ConsumeThread(mQueue, "/tmp/jlog.log");
    mConsumeThread->start();
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
void Logger::addLogFile(const std::string &_LogName, const std::string &_LogFileName)
{
    if(0 == mLogFileList.count(_LogName))
    {
        mLogFileList[_LogName] = _LogFileName;
    }
}
/*
Logger& Logger::operator<< (int _number)
{
    unsigned long tmpThreadID = pthread_self();
    if(0 == mLSList.count(tmpThreadID))
    {
        mLSList[tmpThreadID]= new LogStream();
    }
    *mLSList[tmpThreadID] << _number;
    return *this;
}
Logger& Logger::operator<< (long _number)
{
    unsigned long tmpThreadID = pthread_self();
    if(0 == mSSList.count(tmpThreadID))
    {
        mSSList[tmpThreadID]= new std::stringstream();
    }
    *mSSList[tmpThreadID] << _number;
    return *this;
}
Logger& Logger::operator<< (double _number)
{
    unsigned long tmpThreadID = pthread_self();
    if(0 == mSSList.count(tmpThreadID))
    {
        mSSList[tmpThreadID]= new std::stringstream();
    }
    *mSSList[tmpThreadID] << _number;
    return *this;
}
*/
Logger& Logger::operator<< (std::string _Message)
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
Logger& Logger::operator << (char *_Message)
{
    unsigned long tmpThreadID = pthread_self();
    if(0 == mLSList.count(tmpThreadID))
    {
        mLSList[tmpThreadID]= new LogStream();
    }
    *mLSList[tmpThreadID] << _Message;
    return *this;
}
*/
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

//flags
Logger& INFO(Logger& _logger)
{
    _logger << _logger.getTime();
    _logger << SEPARATOR;
    _logger << _logger.getLevelMarker(Logger::information);
    _logger << SEPARATOR;
    return _logger;
}
Logger& WARNNING(Logger& _logger)
{
    _logger << _logger.getTime();
    _logger << SEPARATOR;
    _logger << _logger.getLevelMarker(Logger::warnning);
    _logger << SEPARATOR;
    return _logger;
}
Logger& ERROR(Logger& _logger)
{
    _logger << _logger.getTime();
    _logger << SEPARATOR;
    _logger << _logger.getLevelMarker(Logger::error);
    _logger << SEPARATOR;
    return _logger;
}
Logger& CRITICAL(Logger& _logger)
{
    _logger << _logger.getTime();
    _logger << SEPARATOR;
    _logger<< _logger.getLevelMarker(Logger::critical);
    _logger << SEPARATOR;
    return _logger;
}
Logger& DEBUG(Logger& _logger)
{
    _logger << _logger.getTime();
    _logger << SEPARATOR;
    _logger << _logger.getLevelMarker(Logger::debug);
    _logger << SEPARATOR;
    return _logger;
}
Logger& NOLEVEL(Logger& _logger)
{
    _logger << _logger.getTime();
    _logger << SEPARATOR;
    _logger << _logger.getLevelMarker(Logger::nolevel);
    _logger << SEPARATOR;
    return _logger;
}
Logger& END_LOGGER(Logger& _logger)
{
    _logger.writeLogToFile();
    return _logger;
}
Logger& SETID(Logger& _logger)
{
    return _logger;
}
