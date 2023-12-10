#include "server.h"

Server::Server(const quint32 port)
{
    this->port = port;
    if (init())
        qDebug() << "Server listen";
    else
        qDebug() << "Server failed";
}

Server::Server()
{
    this->port = DEFAULT_PORT;
}

bool Server::init()
{
    return httpServer.listen(QHostAddress::Any, port);
}


const TextType Server::getTextType(const QString& str)
{
    return str == "text" ? TextType::text : TextType::words;
}

const Language Server::getLanguage(const QString& str)
{
    return str == "ru" ? Language::ru : Language::en;
}


void Server::routeHome(Sql& sql) // Get -> (get text)   Post -> (insert statistic)
{

    httpServer.route(ROUTE_HOME, [&sql](const QHttpServerRequest &request) {
        switch(request.method()) {
        case METHOD_GET:
            if (request.query().isEmpty()) {
                QJsonObject obj = sql.getRandomText(Language::ru, getTextType(DEFAULT_TEXT_TYPE),
                                                    DEFAULT_TEXT_SIZE);
                return obj.isEmpty() ?
                        QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError) :
                        QHttpServerResponse(obj);
            }
            else {
                QJsonObject obj = sql.getRandomText(getLanguage(request.query().queryItems().at(0).second),
                                                    getTextType(request.query().queryItems().at(1).second),
                                                    request.query().queryItems().at(2).second.toInt());
                return obj.isEmpty() ?
                        QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError) :
                        QHttpServerResponse(obj);
            }

        case METHOD_POST:
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
    httpServer.route(ROUTE_SIGNIN, [&sql](const QHttpServerRequest &request) {
        Auth auth;
        switch (request.method()) {
        case METHOD_POST:
            auth = sql.findUser(QJsonDocument::fromJson(request.body().data()).object());
            switch(auth.bool_values) {
            case BoolValues::True:
                return QHttpServerResponse(QJsonObject({{KEY_USER_ID, int(auth.id)}}));
            case BoolValues::Incorrect:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::NotImplemented);
            default:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
            }
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}

void Server::routeSignUp(Sql&  sql) // Post -> (insert User)
{
    httpServer.route(ROUTE_SIGNUP, [&sql](const QHttpServerRequest &request) {
        Auth auth;
        switch (request.method()) {
        case METHOD_POST:
            auth = sql.addUser(QJsonDocument::fromJson(request.body().data()).object());
            switch(auth.bool_values) {
            case BoolValues::True:
                return QHttpServerResponse(QJsonObject({{KEY_USER_ID, int(auth.id)}}));
            case BoolValues::Incorrect:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::NotImplemented);
            default:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
            }
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}


void Server::routeSettingsUsername(Sql& sql)
{
    httpServer.route(ROUTE_SETTINGS_USERNAME, [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_POST:
            switch(sql.changeUsername(QJsonDocument::fromJson(request.body().data()).object())) {
            case BoolValues::True:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
            case BoolValues::Incorrect:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::NotImplemented);
            default:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
            }

        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}

void Server::routeSettingsPassword(Sql& sql)
{
    httpServer.route(ROUTE_SETTINGS_PASSWORD, [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_POST:
            switch(sql.changePassword(QJsonDocument::fromJson(request.body().data()).object())) {
            case BoolValues::True:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
            case BoolValues::Incorrect:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::NotImplemented);
            default:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
            }
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}

void Server::routeSettingsDelete(Sql& sql)
{
    httpServer.route(ROUTE_SETTINGS_DELETE, [&sql](const QHttpServerRequest &request) {
        switch (request.method()) {
        case METHOD_DELETE:
            switch(sql.deleteAccount(QJsonDocument::fromJson(request.body().data()).object())) {
            case BoolValues::True:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::Ok);
            case BoolValues::Incorrect:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::NotImplemented);
            default:
                return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
            }
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}


void Server::routeProfile(Sql& sql) // Get -> (get Profile + get stat)
{
    httpServer.route(ROUTE_PROFILE, [&sql](const QHttpServerRequest &request) {
        QJsonObject obj;
        switch (request.method()) {
        case METHOD_GET:
            obj = sql.getProfileStat(request.query().queryItems().at(0).second.toInt());
            return obj.isEmpty() ?
                QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError) :
                QHttpServerResponse(obj);
        default:
            return QHttpServerResponse(QHttpServerResponse::StatusCode::InternalServerError);
        }
    });
}

