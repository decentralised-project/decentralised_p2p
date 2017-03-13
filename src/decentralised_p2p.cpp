#include "decentralised_p2p.h"

decentralised_p2p::decentralised_p2p(QObject *parent, int incomingPort) :
    QObject(parent)
{
    _incomingPort = incomingPort;
    _server = new QTcpServer(this);
    _clients = new QList<QTcpSocket>();
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
    if(_server->isListening())
        _server->close();
}

void decentralised_p2p::Send(QByteArray data)
{

}

void decentralised_p2p::RequestDnsSeeds()
{
    QHostInfo::lookupHost("dnsseed.decentralised-project.org",
                          this, SLOT(on_dnslookup(QHostInfo)));

//    QHostInfo::lookupHost("dnsseed.evil.center",
//                          this, SLOT(on_dnslookup(QHostInfo)));
}

void decentralised_p2p::on_newconnection()
{
    emit connectionIncoming();
}

void decentralised_p2p::on_dnslookup(QHostInfo e)
{
    if(e.error() == QHostInfo::NoError)
    {
        QList<QHostAddress> addresses = e.addresses();
        if(addresses.size() == 0)
        {
            emit dnsSeedError("No IP Addresses.");
            return;
        }
        for (QList<QHostAddress>::iterator it = addresses.begin() ; it != addresses.end(); ++it)
        {
            QHostAddress h((*it));
                emit dnsSeedReceived((*it).toString());
        }
    }
    else
        emit dnsSeedError(e.errorString());
}
