#include <LogStream.h>
LogStream::LogStream()
{
    mSS.clear();
    mSS.str("");
}
LogStream::~LogStream()
{
}
/*
LogStream& LogStream::operator<< (const std::string &_String)
{
    mSS << _String;
    return *this;
}
LogStream& LogStream::operator << (const char *_Message)
{
    mSS << _Message;
    return *this;
}
*/
std::string LogStream::toString()
{
    return mSS.str();
}
void LogStream::clear()
{
    mSS.str("");
}
