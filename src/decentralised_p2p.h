#ifndef DECENTRALISED_P2P_H
#define DECENTRALISED_P2P_H

#include <QObject>
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include "peer.h"

class decentralised_p2p: public QObject
{
    Q_OBJECT
    public:
        explicit decentralised_p2p(QObject *parent = 0, int incomingPort = 6453);

        void Start();
        void Stop();

        void Send(QByteArray data);
        void RequestDnsSeeds();

    signals:
        void dataReceived(QByteArray data);
        void connectionEstablished();
        void connectionDropped();
        void connectionIncoming();
        void serverStarted(int port);
        void serverError(QString message);
        void dnsSeedReceived(QString ip);
        void dnsSeedError(QString message);

    public slots:

    private slots:
        void on_dnslookup(QHostInfo e);

    private:
        void on_newconnection();

        int _incomingPort;
        QTcpServer* _server;
        QList<QTcpSocket>* _clients;
};

#endif // DECENTRALISED_P2P_H
