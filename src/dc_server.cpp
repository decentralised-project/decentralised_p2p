#include "dc_server.h"

dc_server::dc_server(int port)
{
    isRunning = false;
    incomingPort = port;
}

void dc_server::run()
{
    isRunning = true;

    // todo: do stuff

    isRunning = false;
}

void dc_server::stop()
{

}

bool dc_server::IsRunning()
{
    return isRunning;
}
