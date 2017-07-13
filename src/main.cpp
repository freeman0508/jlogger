#include "Logger.h"
#include "ProduceThread.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    //init all logger resource in main process
    //if there is no this line. sub thread will init. Main thread will stop quick. Maybe sub thread cannot init all resource finished.
    //so init logger is necessary
    INIT_LOGGER;
    LOAD_CONFIG("logger.cfg");
    SET_MAX_LOG_FILE_COUNT(34);
    DUMP_CONFIGER;
    //LOGGER << "TEST" << SETID << INFO << "Start ......" << END_LOGGER;
    INFO_LOGGER_ID("TEST") << "Start 1......" << END_LOGGER;
    INFO_LOGGER_ID("ROOT") << "Start 2......" << END_LOGGER;
    INFO_LOGGER << "Start 3......" << END_LOGGER;
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
        INFO_LOGGER << "I come from main thread." << END_LOGGER;
        //sleep(1);
    }
    INFO_LOGGER << "test INFO_LOGGER 1" << END_LOGGER;
    CRITICAL_LOGGER << "test CRITICAL_LOGGER" << END_LOGGER;
    CRITICAL_LOGGER << "test CRITICAL_LOGGER------- !!!" << END_LOGGER;
    INFO_LOGGER << "test INFO_LOGGER 2" << END_LOGGER;
    WARNNING_LOGGER << "test WARNNING_LOGGER" << END_LOGGER;
    ERROR_LOGGER << "test ERROR_LOGGER" << END_LOGGER;
    DEBUG_LOGGER << "test DEBUG_LOGGER" << END_LOGGER;
    NOLEVEL_LOGGER << "1|WRITE|True|test DEBUG_LOGGER" << END_LOGGER;
    INFO_LOGGER << "ROOT" << SETID << "2|WRITE|True|test DEBUG_LOGGER" << END_LOGGER;
    NOLEVEL_LOGGER << "ROOT" << SETID << "3|WRITE|True|test DEBUG_LOGGER" << END_LOGGER;
    //NOLEVEL_LOGGER_ID("HELLO") << "4|WRITE|True|test DEBUG_LOGGER" << END_LOGGER;
    pt.stop();
    pt1.stop();
    pt2.stop();
    /*
    */
    return 0;
}
