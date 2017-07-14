# jlogger
    Jack's logger. I am a software engineer from China. You know so many Chinese company requir every employee get a English name. My English name is Jack a quite common name in English universe. So I named this little toy project jlogger. It's easy to know it's a logger library with c++. 

The version 1.0 contains features listed below:
    1. Support iostream syntax like: cout << "message ........" << endl; 
    2. Support singlton mode. You could use same logger instance in your one process.
    3. thread-safe
    4. Support roll out multi log file on disk when the current log file size is up to a benchmark. Now the max log file number is 10, and the size benchmark is 1000000 Bytes.

The roadmap in the future:
    1. Support configuration file to set properties such as log file number, size benchmark, datetime format;
    2. Support multi logger id in one process. User could output log message to special log file;

Version 1.1:
    1. Support configure properties by a Configer class. This class just init in process, cannot read configuration file now.
        1.1 can configure MaxLogFileSize, MaxLogFileCount, LogID and LogFilePath mapping
    2. Support multi log files by log id. So far just support only one log id "ROOT". 
        2.1 roll out log file by log id

Version 1.1.0:
    1. support char *, string, int, double, float, long by template;
    2. Support multi log files by log id.  INFO_LOGGER_ID to tell which LogID was used
        INFO_LOGGER_ID("TEST") << "Start 1......" << END_LOGGER;
        INFO_LOGGER_ID("ROOT") << "Start 2......" << END_LOGGER;
    3. read ini configuration file to set properties. call api to reset properties.
        MaxLogFileSize, MaxLogFileCount
    Issue:
        1. 
ConsumeThread::initLogFiles ROOT=>/tmp/jlogger.log
ConsumeThread::initLogFiles TEST=>/tmp/jlogger_test.log

 is not exists!
