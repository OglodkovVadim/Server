#include "sql.h"

Sql::Sql() :
    data_base(QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"))),
    query(QSqlQuery(data_base))
{}

void Sql::init (
    const QString hostName,
    const QString userName,
    const QString password,
    const uint16_t port
)
{
    data_base.setHostName(hostName);
    data_base.setUserName(userName);
    data_base.setPassword(password);
    data_base.setPort(port);
}

void Sql::createTables()
{
    try {
        data_base.open();
        qDebug() << "Data base opened sucessful";

    // create table for users (only users info)
        query.exec(
            "CREATE TABLE users ("
                "id BIGINT PRIMARY KEY,"
                "login VARCHAR(64) NOT NULL,"
                "password VARCHAR(64) NOT NULL,"
                "date_registration DATE DEFAULT CURRENT_DATE"
            ")"
        );

    // create table for statistics (related to the table users by foreigen key user_id)
        query.exec(
            "CREATE TABLE statistics ("
                    "id BIGINT PRIMARY KEY,"
                    "average_speed INT NOT NULL,"
                    "max_speed INT NOT NULL,"
                    "average_count_mistakes INT NOT NULL,"
                    "user_id BIGINT NOT NULL,"
                    "CONSTRAINT user_id_fk FOREIGN KEY (user_id) REFERENCES users (id)"
            ")"
        );

    // create table for complited achivements (related to the table users by foreigen key user_id)
        query.exec(
            "CREATE TABLE complted_achivements ("
                "id BIGINT PRIMARY KEY,"
                "user_id BIGINT NOT NULL,"
                "CONSTRAINT user_id_fk FOREIGN KEY (user_id) REFERENCES users (id)"
            ")"
         );

    // create table for achivements (list of all achivements)
        query.exec(
            "CREATE TABLE achivements ("
                "id BIGINT PRIMARY KEY,"
                "title VARCHAR(64) NOT NULL,"
                "text TEXT NOT NULL,"
                "type INT NOT NULL"
            ")"
        );

    // create table for texts (list of all texts)
        query.exec(
            "CREATE TABLE texts ("
                "id BIGINT PRIMARY KEY,"
                "text TEXT NOT NULL"
            ")"
        );

    } catch (...) {
        qDebug() << data_base.lastError();
    }
}
