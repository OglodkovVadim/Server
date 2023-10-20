#ifndef SQL_H
#define SQL_H

#include "libs.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QRandomGenerator>
#include <QDate>

class Sql
{
public:
    explicit Sql();

    void init (
        const QString,
        const QString,
        const QString,
        const uint16_t
    );

    void createTables();

    void addUser(const QJsonObject&);
    const QJsonObject getRandomText(const uint16_t);
    void addText();

private:
    QSqlDatabase data_base;
    QSqlQuery query;
};

#endif // SQL_H
