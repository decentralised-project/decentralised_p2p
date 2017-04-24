#include "decentralised_p2p.h"

const QString _dnsSeeds[] = { "dnsseed.decentralised-project.org",
                              "dnsseed.evil.center" };

decentralised_p2p::decentralised_p2p(EC_KEY *instanceKey, QObject *parent, int incomingPort) :
    QObject(parent)
    , _networkSession(Q_NULLPTR)
{
    _instanceKey = instanceKey;
    _currentDnsSeedIndex = 0;
    _incomingPort = incomingPort;
    _server = new QTcpServer(this);
    _clients = new QList<dc_peer*>();
    connect(_server, &QTcpServer::newConnection, this, &decentralised_p2p::on_newconnection);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {

        // Get saved network configuration TODO: use settings class
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        _networkSession = new QNetworkSession(config, this);
        connect(_networkSession, &QNetworkSession::opened, this, &decentralised_p2p::on_session_opened);

        // TODO: emit message?

        _networkSession->open();
    }
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
    dc_peer* outgoing = new dc_peer(false, this, new QTcpSocket(this));
    QObject::connect(outgoing, &dc_peer::on_connected, this, &decentralised_p2p::on_outgoing_connected);
    QObject::connect(outgoing, &dc_peer::on_connection_error, this, &decentralised_p2p::on_outgoing_error);
    outgoing->TryConnect(ip, port);
    _clients->append(outgoing);
}

EC_KEY* decentralised_p2p::GetInstanceKey()
{
    return _instanceKey;
}

void decentralised_p2p::on_outgoing_connected()
{
    emit connectionOutgoing();
}

void decentralised_p2p::on_session_opened()
{
    // TODO: Save the used configuration

    // TODO: emit message?
}

void decentralised_p2p::on_outgoing_error(QString message)
{
    emit outgoingError(message);
}

void decentralised_p2p::on_newconnection()
{
    QTcpSocket *clientSocket = _server->nextPendingConnection();

    dc_peer* peer = new dc_peer(true, _crypt-> _instanceKey, this, clientSocket);
    QObject::connect(peer, &dc_peer::on_connection_error, this, &decentralised_p2p::on_outgoing_error);
    _clients->append(peer);

    emit connectionIncoming(peer);
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
