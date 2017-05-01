#ifndef DC_PEER_H
#define DC_PEER_H

#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QtNetwork>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
class QTcpSocket;
class QNetworkSession;

class dc_peer : public QObject
{
    Q_OBJECT
public:
    explicit dc_peer(bool isIncoming, QString localInstancePublicKey, QObject *parent = 0, QTcpSocket *socket = 0);
    virtual ~dc_peer() {
        delete _buffer;
    }

    void TryConnect(QString ip, int port);
    QHostAddress GetRemoteAddress();
    bool IsIncoming();

signals:
    void on_connected();
    void on_connection_error(QString message);
    void on_data_received(dc_peer *sender, QByteArray data);

public slots:

private slots:
    void readTcpData();
    void connectionError();
    void outgoing_connected();

private:
    qint32 arrayToInt(QByteArray source);
    QByteArray intToArray(qint32 source);

    QTcpSocket* _socket;
    QDataStream _in;
    bool _isIncoming;
    qint32 _dataSize;
    QByteArray *_buffer;

    QString _localInstancePublicKey;
    unsigned char* shared_secret;
    int shared_secret_len;
};

#endif // DC_PEER_H
