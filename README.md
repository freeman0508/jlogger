# jlogger
Jack's logger. I am a software engineer from China. You know so many Chinese company requir every employee get a English name. My English name is Jack a quite common name in English universe. So I named this little toy project jlogger. It's easy to know it's a logger library with c++. The first version contains features listed below:
    1. Support iostream syntax like: cout << "message ........" << endl; 
    2. Support singlton mode. You could use same logger instance in your one process.
    3. thread-safe
    4. Support roll out multi log file on disk when the current log file size is up to a benchmark. Now the max log file number is 10, and the size benchmark is 1000000 Bytes.

The roadmap in the future:
    1. Support configuration file to set properties such as log file number, size benchmark, datetime format;
    2. Support multi logger id in one process. User could output log message to special log file;
