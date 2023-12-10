#ifndef SQL_H
#define SQL_H

#include "libs.h"
#include "user.h"
#include "text.h"

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

class Sql : protected User, protected Text
{
public:
    explicit Sql();

    void init (
        const QString,
        const QString,
        const QString,
        const quint32
        );

    void createTables();

    const QJsonObject getRandomText(const TextType, const quint32);
    const QJsonObject generateText(QSqlQuery&, const quint32);
    const QJsonObject generateWords(QSqlQuery&, const quint32);

    const Auth addUser(const QJsonObject&);
    const Auth findUser(const QJsonObject&);

    const void addText();
    const bool addStatistic(const QJsonObject&);

    const BoolValues changeUsername(const QJsonObject&);
    const BoolValues changePassword(const QJsonObject&);
    const BoolValues deleteAccount(const QJsonObject&);

    const QJsonObject getProfileStat(const uint32_t id);



private:
    QSqlDatabase sql_database;
    QSqlQuery query;
};

#endif // SQL_H
