#include "decentralised_p2p.h"

const QString _dnsSeeds[] = { "dnsseed.decentralised-project.org",
                              "dnsseed.evil.center" };

decentralised_p2p::decentralised_p2p(QObject *parent, int incomingPort) :
    QObject(parent)
{
    _currentDnsSeedIndex = 0;
    _incomingPort = incomingPort;
    _server = new QTcpServer(this);
    _clients = new QList<peer*>();
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

void decentralised_p2p::StartOutgoing(QString ip, int port)
{
    peer* outgoing = new peer(this);
    QObject::connect(outgoing, &peer::on_connected, this, &decentralised_p2p::on_outgoing_connected);
    QObject::connect(outgoing, &peer::on_connectionError, this, &decentralised_p2p::on_outgoing_error);
    outgoing->TryConnect(ip, port);
    _clients->append(outgoing);
}

void decentralised_p2p::on_outgoing_connected()
{
    emit connectionOutgoing();
}

void decentralised_p2p::on_outgoing_error(QString message)
{
    emit outgoingError(message);
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
