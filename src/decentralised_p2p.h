#ifndef DECENTRALISED_P2P_H
#define DECENTRALISED_P2P_H

#include <QObject>
#include <QTcpServer>
#include "peer.h"

class decentralised_p2p: public QObject
{
    Q_OBJECT
    public:
        explicit decentralised_p2p(QObject *parent = 0, int incomingPort = 6453);

        void Start();
        void Stop();

        void Send(QByteArray data);

    signals:
        void dataReceived(QByteArray data);
        void connectionEstablished();
        void connectionDropped();
        void connectionIncoming();
        void serverStarted(int port);
        void serverError(QString message);

    public slots:

    private:

        void on_newconnection();

        int _incomingPort;
        QTcpServer* _server;
};

#endif // DECENTRALISED_P2P_H
