#ifndef DC_PEER_H
#define DC_PEER_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QtNetwork>

class QTcpSocket;
class QNetworkSession;

class dc_peer : public QObject
{
    Q_OBJECT
public:
    explicit dc_peer(bool isIncoming, QObject *parent = 0, QTcpSocket *socket = 0);

    void TryConnect(QString ip, int port);
    QHostAddress GetRemoteAddress();
    bool IsIncoming();
    void WaitForData();

signals:
    void on_connected();
    void on_connection_error(QString message);

public slots:

private slots:
    void on_readTcpData();
    void on_error();

private:
    QTcpSocket* _socket;
    QDataStream _in;
    bool _isIncoming;

    unsigned char* shared_secret;
    int shared_secret_len;
};

#endif // DC_PEER_H
