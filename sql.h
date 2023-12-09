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

    const bool addUser(const QJsonObject&);
    const bool findUser(const QJsonObject&);

    const bool addText();
    const bool addStatistic(const QJsonObject&);

    const DangerousValues changeUsername(const QJsonObject&);
    const bool changePassword(const QJsonObject&);
    const DangerousValues deleteAccount(const QJsonObject&);

    const QJsonObject getProfileStat(const uint32_t id);



private:
    QSqlDatabase sql_database;
    QSqlQuery query;
};

#endif // SQL_H
