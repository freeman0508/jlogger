#ifndef LOGSTREAM_H
#define LOGSTREAM_H
#include <iostream>
#include <sstream>
class LogStream
{
    public:
        LogStream();
        ~LogStream();
        template <class T> LogStream& operator << (const T &_Message)
        {
            mSS << _Message;
            return *this;
        }
        //LogStream& operator << (const std::string &_String);
        //LogStream& operator << (const char *_Message);
        std::string toString();
        void clear();
    private:
        std::stringstream mSS;
        std::string mLevelName;

};
#endif
