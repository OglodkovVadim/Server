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

QJsonObject Server::routeHome(Sql &sql)
{
    httpServer->route("/", [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_GET:
            qDebug() << request.query().queryItems();

            break;
        case METHOD_POST:
            qDebug() << sql.getRandomText(TextType::words, request.body().at(1));
            break;
        }
        return "Ok";
    });

    return {
        {RESPONSE_TEXT, RESPONSE_CODE_BAD}
    };
}

