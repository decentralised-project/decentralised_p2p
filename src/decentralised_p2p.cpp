#include "decentralised_p2p.h"

const QString _dnsSeeds[] = { "dnsseed.decentralised-project.org",
                              "dnsseed.evil.center" };

decentralised_p2p::decentralised_p2p(QObject *parent, int incomingPort) :
    QObject(parent)
{
    _currentDnsSeedIndex = 0;
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
    QHostInfo::lookupHost(_dnsSeeds[_currentDnsSeedIndex],
                          this, SLOT(on_dnslookup(QHostInfo)));
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
        for (int i = 0; i < addresses.size(); ++i)
        {
            emit dnsSeedReceived(addresses[i].toString());
        }
    }
    else
        emit dnsSeedError(e.errorString());
}
