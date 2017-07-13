#include <LogStream.h>
LogStream::LogStream()
{
    mSS.clear();
    mSS.str("");
}
LogStream::~LogStream()
{
}
LogStream &LogStream::operator<< (std::string _String)
{
    mSS << _String;
    return *this;
}
std::string LogStream::toString()
{
    return mSS.str();
}
void LogStream::clear()
{
    mSS.str("");
}
