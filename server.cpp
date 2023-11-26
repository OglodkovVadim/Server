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


const TextType getTextType(const QString& str)
{
    return str == "text" ? TextType::text : TextType::words;
}



void Server::routeHome(Sql& sql) // Get -> (get text)   Post -> (insert statistic)
{

    httpServer->route(ROUTE_HOME, [&sql](const QHttpServerRequest &request) {
        QJsonObject return_obj;
        if (request.query().isEmpty())
            return return_obj;
        else {
            switch (request.method()) {
            case METHOD_GET:
                return_obj = sql.getRandomText(
                    getTextType(request.query().queryItems().at(0).second),
                    request.query().queryItems().at(1).second.toInt()
                );
                break;

            case METHOD_POST:
                return_obj = {{RESPONSE_TEXT, RESPONSE_CODE_OK}};
                break;

            default:
                return_obj = {
                    {RESPONSE_TEXT, RESPONSE_CODE_BAD}
                };
                break;
            }
            return return_obj;
        }
    });
}

void Server::routeSignIn(Sql& sql) // ??? Get/Post -> ()
{
    httpServer->route(ROUTE_SIGNIN, [&sql](const QHttpServerRequest &request) {
        QJsonObject return_obj;
        switch (request.method()) {
        case METHOD_POST:
            return sql.findUser(QJsonDocument::fromJson(request.body().data()).object()) ?
                       QHttpServerResponse(QHttpServerResponse::StatusCode::Ok) :
                       QHttpServerResponse(QHttpServerResponse::StatusCode::BadRequest);
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::BadRequest);
        }
    });
}

void Server::routeSignUp(Sql&  sql) // Post -> (insert User)
{
    httpServer->route(ROUTE_SIGNUP, [&sql](const QHttpServerRequest &request) {
        QJsonObject return_obj;
        switch (request.method()) {
        case METHOD_POST:
            return_obj = sql.addUser(QJsonDocument::fromJson(request.body().data()).object());
            break;
        default:
            return_obj = {
                {RESPONSE_TEXT, RESPONSE_CODE_BAD}
            };
            break;
        }
        return return_obj;
    });
}

void Server::routeSettings(Sql& sql) // Post -> (insert settings)
{

}

void Server::routeStatistic(Sql& sql) // Get -> (get statistics)
{

}

void Server::routeProfile(Sql& sql) // Get -> (get Profile)
{

}
