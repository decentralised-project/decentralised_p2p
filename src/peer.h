#ifndef PEER_H
#define PEER_H

#include <QObject>

class peer : public QObject
{
    Q_OBJECT
public:
    explicit peer(QObject *parent = 0);

signals:

public slots:
};

#endif // PEER_H