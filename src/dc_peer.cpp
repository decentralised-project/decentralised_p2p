#include "dc_peer.h"

dc_peer::dc_peer(bool isIncoming, EC_POINT* localInstancePublicKey, QObject *parent, QTcpSocket *socket) : QObject(parent)
  , _socket(socket)
{
    _isIncoming = isIncoming;
    _localInstancePublicKey = localInstancePublicKey;

    _in.setDevice(_socket);
    _in.setVersion(QDataStream::Qt_4_0);

    //connect(_socket, SIGNAL(readyRead()), SLOT(readTcpData()));
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(_socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
                this, &dc_peer::connectionError);
    connect(_socket, &QTcpSocket::connected, this, &dc_peer::outgoing_connected);
    connect(_socket, &QTcpSocket::readyRead, this, &dc_peer::readTcpData);
}

void dc_peer::TryConnect(QString ip, int port)
{
    _socket->abort();
    _socket->connectToHost(ip, port);
}

QHostAddress dc_peer::GetRemoteAddress()
{
    return _socket->peerAddress();
}

bool dc_peer::IsIncoming()
{
    return _isIncoming;
}

void dc_peer::outgoing_connected()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << _localInstancePublicKey;

    _socket->write(block);
}

void dc_peer::readTcpData()
{
    _in.startTransaction();

    QByteArray data;
    _in >> data;

    if (!_in.commitTransaction())
        return;
}

void dc_peer::connectionError()
{
    QString msg = _socket->errorString();
    emit on_connection_error(msg);
}
