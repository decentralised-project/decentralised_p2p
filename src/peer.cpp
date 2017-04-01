#include "peer.h"

peer::peer(QObject *parent) : QObject(parent)
  , _socket(new QTcpSocket(this))
  , _networkSession(Q_NULLPTR)
{
    _in.setDevice(_socket);
    _in.setVersion(QDataStream::Qt_4_0);

    connect(_socket, SIGNAL(readyRead()), SLOT(readTcpData()));
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(_socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
                this, &peer::on_error);

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
        connect(_networkSession, &QNetworkSession::opened, this, &peer::sessionOpened);

        // TODO: emit message?

        _networkSession->open();
    }
}

void peer::sessionOpened()
{
    // TODO: Save the used configuration (should prob be up a level?)

    // TODO: emit message?
}

void peer::TryConnect(QString ip, int port)
{
    _socket->abort();
    _socket->connectToHost(ip, port);
}

void peer::on_readTcpData()
{

}

void peer::on_error(QAbstractSocket::SocketError err)
{
    QString msg = _socket->errorString();
    emit on_connectionError(msg);
}
