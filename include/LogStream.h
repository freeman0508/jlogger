#ifndef LOGSTREAM_H
#define LOGSTREAM_H
#include <iostream>
#include <sstream>
class LogStream
{
    public:
        LogStream();
        ~LogStream();
        LogStream& operator << (std::string _String);
        std::string toString();
        void clear();
    private:
        std::stringstream mSS;
        std::string mLevelName;

};
#endif
