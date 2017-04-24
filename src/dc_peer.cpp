#include "dc_peer.h"

dc_peer::dc_peer(bool isIncoming, QObject *parent, QTcpSocket *socket) : QObject(parent)
  , _socket(socket)
{
    _isIncoming = isIncoming;

    _in.setDevice(_socket);
    _in.setVersion(QDataStream::Qt_4_0);

    //connect(_socket, SIGNAL(readyRead()), SLOT(readTcpData()));
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(_socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
                this, &dc_peer::on_error);
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

void dc_peer::WaitForData()
{
    _in.startTransaction();

    QByteArray data;
    _in >> data;

    if (!_in.commitTransaction())
        return;
}

void dc_peer::on_readTcpData()
{

}

void dc_peer::on_error()
{
    QString msg = _socket->errorString();
    emit on_connection_error(msg);
}
