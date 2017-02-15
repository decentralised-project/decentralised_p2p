#ifndef DC_SERVER_H
#define DC_SERVER_H

#include <QRunnable>

class dc_server : public QRunnable
{
public:
    dc_server();

    void run();

    void stop();

    bool IsRunning();

private:
    bool isRunning;
};

#endif // DC_SERVER_H
