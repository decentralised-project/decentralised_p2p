#ifndef DECENTRALISED_P2P_H
#define DECENTRALISED_P2P_H

#include <QObject>
#include <QList>
#include <QTcpServer>
#include <QHostInfo>
#include <QSignalMapper>
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
        void StartOutgoing(QString ip, int port);

    signals:
        void dataReceived(QByteArray data);
        void connectionEstablished();
        void connectionDropped();
        void connectionIncoming();
        void connectionOutgoing();
        void serverStarted(int port);
        void serverError(QString message);
        void dnsSeedReceived(QString ip);
        void dnsSeedError(QString message);
        void outgoingError();

    public slots:

    private slots:
        void on_dnslookup(QHostInfo e);
        void on_outgoing_connected();
        void on_outgoing_error();

    private:
        void on_newconnection();

        int _currentDnsSeedIndex;
        int _incomingPort;
        QTcpServer* _server;
        QList<peer*>* _clients;
};

#endif // DECENTRALISED_P2P_H
