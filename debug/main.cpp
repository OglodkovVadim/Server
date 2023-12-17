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

#include "backendtest.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTest::qExec(new BackendTest);

    return 0;
    return app.exec();
}
