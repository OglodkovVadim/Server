#include "server.h"

Server::Server(const quint32 port)
{
    this->port = port;
    httpServer = QSharedPointer<QHttpServer>(new QHttpServer());
}

Server::Server()
{
    this->port = DEFAULT_PORT;
    httpServer = QSharedPointer<QHttpServer>(new QHttpServer());
}

bool Server::init()
{
    return httpServer->listen(QHostAddress::Any, port);
}

QJsonObject Server::routeHome()
{
    httpServer->route(ROUTE_HOME, [](const QHttpServerRequest &request) {
        qDebug() << request.body().data();
        switch (request.method()) {
            case METHOD_POST:
                if (request.body().contains("12345")) {
                        qDebug() << "FAJPFOHFPOBFAF";
                    }
                break;
        }
        return "Cool";
    });

    return {
        {RESPONSE_TEXT, RESPONSE_CODE_BAD}
    };
}
