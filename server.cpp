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
        switch(request.method()) {
        case METHOD_GET:
            if (request.query().isEmpty())
                return QHttpServerResponse(
                    sql.getRandomText(getTextType(DEFAULT_TEXT_TYPE),
                                      DEFAULT_TEXT_SIZE),
                    QHttpServerResponder::StatusCode::Ok);
            else
                return QHttpServerResponse(
                    sql.getRandomText (getTextType(
                        request.query().queryItems().at(0).second),
                        request.query().queryItems().at(1).second.toInt()),
                    QHttpServerResponder::StatusCode::Ok);

        case METHOD_POST:
            qDebug() << "Post";
            return sql.addStatistic(QJsonDocument::fromJson(request.body().data()).object()) ?
                       QHttpServerResponse(QHttpServerResponse::StatusCode::Ok) :
                       QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}


void Server::routeSignIn(Sql& sql) // ??? Get/Post -> ()
{
    httpServer->route(ROUTE_SIGNIN, [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_POST:
            return sql.findUser(QJsonDocument::fromJson(request.body().data()).object()) ?
                       QHttpServerResponse(QHttpServerResponse::StatusCode::Ok) :
                       QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}

void Server::routeSignUp(Sql&  sql) // Post -> (insert User)
{
    httpServer->route(ROUTE_SIGNUP, [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_POST:
            return sql.addUser(QJsonDocument::fromJson(request.body().data()).object()) ?
                       QHttpServerResponse(QHttpServerResponse::StatusCode::Ok) :
                       QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}


void Server::routeSettingsUsername(Sql& sql)
{
    httpServer->route(ROUTE_SETTINGS_USERNAME, [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_POST:
            switch(sql.changeUsername(QJsonDocument::fromJson(request.body().data()).object())) {
            case DangerousValues::True:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
            case DangerousValues::False:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
            case DangerousValues::Incorrect:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Continue);
            }

        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}

void Server::routeSettingsPassword(Sql& sql)
{
    httpServer->route(ROUTE_SETTINGS_PASSWORD, [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_POST:
            return sql.changePassword(QJsonDocument::fromJson(request.body().data()).object()) ?
                       QHttpServerResponse(QHttpServerResponse::StatusCode::Ok) :
                       QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}

void Server::routeSettingsDelete(Sql& sql)
{
    httpServer->route(ROUTE_SETTINGS_DELETE, [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_DELETE:
            switch(sql.deleteAccount(QJsonDocument::fromJson(request.body().data()).object())) {
            case DangerousValues::True:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
            case DangerousValues::False:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
            case DangerousValues::Incorrect:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Continue);
            }
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}


void Server::routeProfile(Sql& sql) // Get -> (get Profile + get stat)
{
    httpServer->route(ROUTE_PROFILE, [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_GET:
            return QHttpServerResponse(sql.getProfileStat(request.query().queryItems().at(0).second.toInt()));
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}



void Server::routeStatistic(Sql& sql) // Get -> (get statistics)
{

}
