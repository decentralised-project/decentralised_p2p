#ifndef DECENTRALISED_P2P_H
#define DECENTRALISED_P2P_H

#include <QObject>

class decentralised_p2p: public QObject
{
    Q_OBJECT
    public:
        explicit decentralised_p2p(QObject *parent = 0);

        void Start();
        void Stop();

        void Send(QByteArray data);

    signals:
        void dataReceived(QByteArray data);
        void connectionEstablished();
        void connectionDropped();

    public slots:

    private:
};

#endif // DECENTRALISED_P2P_H
