#include <QHttpServer>
#include <QHttpServerResponse>
#include <QtHttpServerDepends>

#include <QCoreApplication>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVector>

#define KEY_ID "id"
#define KEY_TEXT "text"
#define KEY_COUNT "countWords"
#define KEY_LOGIN "login"
#define KEY_PASSWORD "password"
#define KEY_SOLVED_TEXTS "solvedTexts"

#include "sql.h"
#include "server.h"

const QString hostName = "127.0.0.1";
const QString userName = "postgres";
const QString password = "1234";
const uint16_t port = 5432;


using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Server server;
    Sql sql;

    server.init();
    sql.init(hostName, userName, password, port);
    sql.createTables();

    sql.addText(Language::ru);
    sql.addText(Language::en);
    sql.addWords(Language::ru);
    sql.addWords(Language::en);

    server.routeHome(sql);
    server.routeSignIn(sql);
    server.routeSignUp(sql);
    server.routeProfile(sql);
    server.routeSettingsUsername(sql);
    server.routeSettingsPassword(sql);
    server.routeSettingsDelete(sql);

    return app.exec();
}
