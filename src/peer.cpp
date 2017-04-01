#include "peer.h"

peer::peer(QObject *parent) : QObject(parent)
{

}

void peer::TryConnect(QString ip, int port)
{
    _socket = new QTcpSocket();
    connect(_socket, SIGNAL(readyRead()), SLOT(readTcpData()));
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(_socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
                this, &peer::on_error);

    _socket->connectToHost(ip, port);

    if(_socket->waitForConnected())
    {
        // TODO: send inital data/msg
        emit on_connected();
    }
}

void peer::on_readTcpData()
{

}

void peer::on_error(QAbstractSocket::SocketError err)
{
    emit on_connectionError();
}
