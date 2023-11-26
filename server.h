#ifndef SERVER_H
#define SERVER_H

#include <QHttpServer>
#include <QSharedPointer>

#include "sql.h"

#define DEFAULT_PORT 80

#define ROUTE_HOME "/"
#define ROUTE_SIGNIN "/signIn/"
#define ROUTE_SIGNUP "/signUp/"
#define ROUTE_SETTINGS "/settings/"
#define ROUTE_STATISTIC "/statistic/"
#define ROUTE_PROFILE "/profile/"

#define METHOD_GET QHttpServerRequest::Method::Get
#define METHOD_POST QHttpServerRequest::Method::Post
#define METHOD_DELETE QHttpServerRequest::Method::Delete

class Server
{
public:
    Server();
    Server(const quint32);

    bool init ();

    void routeHome(Sql&);
    void routeSignIn(Sql&);
    void routeSignUp(Sql&);
    void routeSettings(Sql&);
    void routeStatistic(Sql&);
    void routeProfile(Sql&);

private:
    quint16 port;
    QSharedPointer<QHttpServer> httpServer;
};

#endif // SERVER_H
