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
        const uint16_t
    );

    void createTables();

    const QJsonObject getRandomText(const TextType, const uint16_t);
    void addUser(const QJsonObject&);
    void addText();

private:
    QSqlDatabase data_base;
    QSqlQuery query;
};

#endif // SQL_H
