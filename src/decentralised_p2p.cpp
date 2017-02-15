#include "decentralised_p2p.h"

decentralised_p2p::decentralised_p2p(QObject *parent) :
    QObject(parent)
{
    server = new dc_server();
}

void decentralised_p2p::Start()
{
    if(!server->IsRunning())
        QThreadPool::globalInstance()->start(server);
}

void decentralised_p2p::Stop()
{
    if(server->IsRunning())
        server->stop();

    QThreadPool::globalInstance()->waitForDone(5000);
}

void decentralised_p2p::Send(QByteArray data)
{

}
