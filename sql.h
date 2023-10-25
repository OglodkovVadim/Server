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
    QJsonObject addUser(const QJsonObject&);
    QJsonObject addText();

private:
    QSqlDatabase data_base;
    QSqlQuery query;
};

#endif // SQL_H
