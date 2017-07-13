#include "Logger.h"
#include "ProduceThread.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    INFO_LOGGER << "Start ......" << END_LOGGER;
    ProduceThread pt;
    pt.start();
    ProduceThread pt1;
    pt1.start();
    ProduceThread pt2;
    pt2.start();
    //for(int i=0; i < 100; i++)
    for(int i=0; i < 5000000; i++)
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
    NOLEVEL_LOGGER << "4|WRITE|True|test DEBUG_LOGGER" << END_LOGGER;
    pt.stop();
    pt1.stop();
    pt2.stop();
    /*
    */
    return 0;
}
