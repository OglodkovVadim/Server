#ifndef SQL_H
#define SQL_H

#include "libs.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QRandomGenerator>
#include <QDate>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#define SQL_HOST_NAME "127.0.0.1"
#define SQL_USER_NAME "postgres"
#define SQL_PASSWORD  "1234"
#define SQL_PORT      5432

class Sql
{
public:
    explicit Sql();

    void init();

    void createTables();

    const QJsonObject getRandomText(const Language, const TextType, const quint32);
    const QJsonObject generateText(const Language, const quint32);
    const QJsonObject generateWords(const Language, const quint32);

    const Auth addUser(const QJsonObject&);
    const Auth findUser(const QJsonObject&);
    const bool addStatistic(const QJsonObject&);

    const void addText(const Language);
    const void addWords(const Language);

    const BoolValues changeUsername(const QJsonObject&);
    const BoolValues changePassword(const QJsonObject&);
    const BoolValues deleteAccount(const QJsonObject&);

    const QJsonObject getProfileStat(const uint32_t id);

    void clenUpDB();

    ~Sql();

private:
    QSqlDatabase sql_database;
    QSqlQuery query;
};

#endif // SQL_H
