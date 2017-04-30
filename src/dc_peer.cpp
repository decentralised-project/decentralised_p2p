#include "dc_peer.h"

dc_peer::dc_peer(bool isIncoming, QString localInstancePublicKey, QObject *parent, QTcpSocket *socket) : QObject(parent)
  , _socket(socket)
  , _buffer(new QByteArray())
{
    _isIncoming = isIncoming;
    _localInstancePublicKey = localInstancePublicKey;
    _dataSize = 0;

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
    _socket->write(intToArray(_localInstancePublicKey.size()));
    _socket->write(_localInstancePublicKey.toLocal8Bit());
}

void dc_peer::readTcpData()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());

    while (socket->bytesAvailable() > 0)
    {
        _buffer->append(socket->readAll());
        int buffSize = _buffer->size();

        if (_dataSize == 0 && buffSize >= 4)
        {
            _dataSize = arrayToInt(_buffer->mid(0, 4));
            _buffer->remove(0, 4);
        }

        if (_dataSize > 0 && buffSize >= _dataSize)
        {
            QByteArray data = _buffer->mid(0, _dataSize);
            _buffer->remove(0, _dataSize);
            _dataSize = 0;
            emit on_data_received(data);
        }
    }
}

void dc_peer::connectionError()
{
    QString msg = _socket->errorString();
    emit on_connection_error(msg);
}

qint32 dc_peer::arrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

QByteArray dc_peer::intToArray(qint32 source)
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
