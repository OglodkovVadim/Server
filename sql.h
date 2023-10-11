#ifndef SQL_H
#define SQL_H

#define KEY_ID "id"
#define KEY_TEXT "text"
#define KEY_TYPE "type"
#define KEY_LOGIN "login"
#define KEY_TITLE "title"
#define KEY_USER_ID "user_id"
#define KEY_PASSWORD "password"
#define KEY_COUNT "count_words"
#define KEY_MAX_SPEED "max_speed"
#define KEY_AVERAGE_SPEED "average_speed"
#define KEY_AVERAGE_COUNT_MISTAKES "average_count_mistakes"
#define KEY_DATE_REGISTRATION "date_registration"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSharedPointer>

class Sql
{
public:
    Sql();

    void init (
        const QString,
        const QString,
        const QString,
        const uint16_t
    );

    void createTables();

private:
    QSqlDatabase data_base;
    QSqlQuery query;
};

#endif // SQL_H
