#ifndef DC_SERVER_H
#define DC_SERVER_H

#include <QRunnable>

class dc_server : public QRunnable
{
public:
    dc_server(int port);

    void run();

    void stop();

    bool IsRunning();

private:
    bool isRunning;
    int incomingPort;
};

#endif // DC_SERVER_H
