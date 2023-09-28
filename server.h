#ifndef SERVER_H
#define SERVER_H

#include <QHttpServer>
#include <QHttpServerResponse>
#include <QtHttpServerDepends>

#include <QObject>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

private:
    QHttpServer server;

signals:

};

#endif // SERVER_H
