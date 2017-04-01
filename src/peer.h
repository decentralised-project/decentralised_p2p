#ifndef PEER_H
#define PEER_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>

class peer : public QObject
{
    Q_OBJECT
public:
    explicit peer(QObject *parent = 0);

    void TryConnect(QString ip, int port);

signals:
    void on_connected();
    void on_connectionError();

public slots:

private slots:
    void on_readTcpData();
    void on_error(QAbstractSocket::SocketError err);

private:
    QTcpSocket* _socket;
};

#endif // PEER_H
