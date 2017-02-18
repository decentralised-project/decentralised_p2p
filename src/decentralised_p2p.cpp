#include "decentralised_p2p.h"

decentralised_p2p::decentralised_p2p(QObject *parent, int incomingPort) :
    QObject(parent)
{
    _incomingPort = incomingPort;
    _server = new QTcpServer(this);
    connect(_server, &QTcpServer::newConnection, this, &decentralised_p2p::on_newconnection);
}

void decentralised_p2p::Start()
{
    if(!_server->listen(QHostAddress::Any, _incomingPort))
    {
        emit serverError(_server->errorString());
        return;
    }

    emit serverStarted(_incomingPort);
}

void decentralised_p2p::Stop()
{
    //if(server->IsRunning())
    //    server->stop();

    //QThreadPool::globalInstance()->waitForDone(5000);
}

void decentralised_p2p::Send(QByteArray data)
{

}

void decentralised_p2p::on_newconnection()
{
    emit connectionIncoming();
}
