#ifndef DECENTRALISED_P2P_H
#define DECENTRALISED_P2P_H

#include <QObject>
#include <QList>
#include <QTcpServer>
#include <QHostInfo>
#include <QSignalMapper>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include "dc_peer.h"
#include "decentralised_crypt.h"
#include "dc_connection_dropped.h"

class decentralised_p2p: public QObject
{
    Q_OBJECT
    public:
        explicit decentralised_p2p(EC_KEY *instanceKey, decentralised_crypt *crypt, QObject *parent = 0, int incomingPort = 6453);

        void Start();
        void Stop();

        void Send(QByteArray data);
        void RequestDnsSeeds();
        void StartOutgoing(QString ip, int port);
        EC_KEY* GetInstanceKey();

    signals:
        void dataReceived(QByteArray data);
        void connectionIncoming(dc_peer* peer);
        void connectionOutgoing();
        void connectionDropped(dc_connection_dropped reason);
        void serverStarted(int port);
        void serverError(QString message);
        void dnsSeedReceived(QString ip);
        void dnsSeedError(QString message);
        void outgoingError(QString message);

    public slots:

    private slots:
        void on_dnslookup(QHostInfo e);
        void on_outgoing_connected();
        void on_error(QString message);
        void on_session_opened();
        void on_data_received(dc_peer *sender, QByteArray data);

    private:
        void on_newconnection();

        int _currentDnsSeedIndex;
        int _incomingPort;
        QTcpServer* _server;
        QList<dc_peer*>* _clients;
        EC_KEY* _instanceKey;
        QNetworkSession* _networkSession;
        decentralised_crypt *_crypt;
};

#endif // DECENTRALISED_P2P_H
