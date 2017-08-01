#include "Logger.h"
#include "ProduceThread.h"
#include <unistd.h>

void mostSimple()
{
    INIT_LOGGER;
    INFO_LOGGER << "mostSimple" << END_LOGGER;
}
void configSet()
{
    INIT_LOGGER;
    LOAD_CONFIG("logger.cfg");
    SET_MAX_LOG_FILE_COUNT(34);
    SET_MAX_LOG_FILE_SIZE(1000000);
    INFO_LOGGER << "configSet" << END_LOGGER;
    DUMP_CONFIGER;
}
void loggerIDSet()
{
    //ROOT, must be set before first log information output, otherwise it will not work
    //ADD_LOGGER_ID("ROOT", "/tmp/jlog.fmsh.log");
    ADD_LOGGER_ID("TEST", "/tmp/jlog.test.log");
    ADD_LOGGER_ID("FMSH", "/tmp/jlog.fmsh.log");
    ADD_LOGGER_ID("ACETEC", "/tmp/jlog.ace.log");
    INFO_LOGGER_ID("TEST") << "Start 1......" << END_LOGGER;
    INFO_LOGGER_ID("FMSH") << "Start 2......" << END_LOGGER;
    INFO_LOGGER_ID("ACETEC") << "acetec test add log file" << END_LOGGER;
    INFO_LOGGER << "Start 3......" << END_LOGGER;
}
void loggerLevelTest()
{
    INFO_LOGGER << "test int: " << 230 << END_LOGGER;
    INFO_LOGGER << "test double: " << 2.14 << END_LOGGER;
    char *tmpChar = "xxxxxxxx";
    INFO_LOGGER << "test char: " << tmpChar << END_LOGGER;
    INFO_LOGGER << "test INFO_LOGGER 1" << END_LOGGER;
    CRITICAL_LOGGER << "test CRITICAL_LOGGER" << END_LOGGER;
    CRITICAL_LOGGER << "test CRITICAL_LOGGER------- !!!" << END_LOGGER;
    INFO_LOGGER << "test INFO_LOGGER 2" << END_LOGGER;
    WARNNING_LOGGER << "test WARNNING_LOGGER" << END_LOGGER;
    ERROR_LOGGER << "test ERROR_LOGGER" << END_LOGGER;
    DEBUG_LOGGER << "test DEBUG_LOGGER" << END_LOGGER;
    NOLEVEL_LOGGER << "1|WRITE|True|test DEBUG_LOGGER" << END_LOGGER;
    NOLEVEL_LOGGER_ID("ACETEC") << "test ACETEC" << END_LOGGER;
    NOLEVEL_LOGGER_ID("TEST") << "test TEST" << END_LOGGER;
    NOLEVEL_LOGGER_ID("FMSH") << "test FMSH" << END_LOGGER;
}
void multiThreadsTest()
{
    //init all logger resource in main process
    //if there is no this line. sub thread will init. Main thread will stop quick. Maybe sub thread cannot init all resource finished.
    //so init logger is necessary

    ProduceThread pt;
    pt.start();
    ProduceThread pt1;
    pt1.start();
    ProduceThread pt2;
    pt2.start();
    for(int i=0; i < 100; i++)
    //for(int i=0; i < 5000000; i++)
    //while(true)
    {
        loggerLevelTest();
    }
    sleep(1);
    pt.stop();
    pt1.stop();
    pt2.stop();

}

int main(int argc, char **argv)
{
    mostSimple();
    configSet();
    loggerIDSet();
    loggerLevelTest();
    multiThreadsTest();
    return 0;
}
