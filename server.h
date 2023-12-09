#ifndef SERVER_H
#define SERVER_H

#include <QHttpServer>
#include <QHttpServerResponse>
#include <QtHttpServerDepends>

#include <QObject>
#include <QSharedPointer>

#include "sql.h"

#define DEFAULT_PORT 80

#define ROUTE_HOME "/"
#define ROUTE_SIGNIN "/signIn/"
#define ROUTE_SIGNUP "/signUp/"
#define ROUTE_STATISTIC "/statistic/"
#define ROUTE_PROFILE "/profile/"
#define ROUTE_SETTINGS_USERNAME "/settings/username/"
#define ROUTE_SETTINGS_PASSWORD "/settings/password/"
#define ROUTE_SETTINGS_DELETE "/settings/delete/"

#define METHOD_GET QHttpServerRequest::Method::Get
#define METHOD_POST QHttpServerRequest::Method::Post
#define METHOD_DELETE QHttpServerRequest::Method::Delete

#define DEFAULT_TEXT_TYPE "text"
#define DEFAULT_TEXT_SIZE 20

class Server
{
public:
    Server();
    Server(const quint32);

    bool init ();

    void routeHome(Sql&);
    void routeSignIn(Sql&);
    void routeSignUp(Sql&);
    void routeStatistic(Sql&);
    void routeProfile(Sql&);
    void routeSettingsUsername(Sql&);
    void routeSettingsPassword(Sql&);
    void routeSettingsDelete(Sql&);

private:
    quint16 port;
    QSharedPointer<QHttpServer> httpServer;
};

#endif // SERVER_H
