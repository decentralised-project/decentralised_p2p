#ifndef DC_P2P_H
#define DC_P2P_H

#include <QObject>

class dc_p2p: public QObject
{
    Q_OBJECT
    public:
        explicit dc_p2p(QObject *parent = 0);

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

#endif // DC_P2P_H
