#include "sql.h"

Sql::Sql() :
    sql_database(QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"))),
    query(QSqlQuery(sql_database))
{}

void Sql::init (
    const QString hostName,
    const QString userName,
    const QString password,
    const quint32 port
)
{
    try {
        sql_database.setHostName(hostName);
        sql_database.setUserName(userName);
        sql_database.setPassword(password);
        sql_database.setPort(port);
        sql_database.open();
        qDebug() << "Data base opened sucessful";
    } catch (...) {
    qDebug() << sql_database.lastError();
    }
}


// ???? This func will work only once
void Sql::createTables()
{
    try {
    // create table for users (only users info)
        query.exec(
            "CREATE TABLE IF NOT EXISTS users ("
                "id BIGINT PRIMARY KEY,"
                "login VARCHAR(64) NOT NULL,"
                "password VARCHAR(64) NOT NULL,"
                "date_registration DATE DEFAULT CURRENT_DATE"
            ")"
        );

    // create table for statistics (related to the table users by foreigen key user_id)
        query.exec(
            "CREATE TABLE IF NOT EXISTS statistics ("
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
            "CREATE TABLE IF NOT EXISTS complted_achivements ("
                "id BIGINT PRIMARY KEY,"
                "user_id BIGINT NOT NULL,"
                "CONSTRAINT user_id_fk FOREIGN KEY (user_id) REFERENCES users (id)"
            ")"
         );

    // create table for achivements (list of all achivements)
        query.exec(
            "CREATE TABLE IF NOT EXISTS achivements ("
                "id BIGINT PRIMARY KEY,"
                "title VARCHAR(64) NOT NULL,"
                "text TEXT NOT NULL,"
                "type INT NOT NULL"
            ")"
        );

    // create table for texts (list of all texts)
        query.exec(
            "CREATE TABLE IF NOT EXISTS texts ("
                "id BIGINT PRIMARY KEY,"
                "text TEXT NOT NULL,"
                "count_words INT NOT NULL"
            ")"
        );

    // create table for words (list of words)
        query.exec("CREATE TABLE IF NOT EXISTS words (text TEXT NOT NULL)");

    } catch (...) {
        qDebug() << sql_database.lastError();
    }
}


QJsonObject Sql::addUser(const QJsonObject& object)
{
    query.exec();
    query.prepare("INSERT INTO users (id, login, password, date_registration) "
                  "VALUES (?, ?, ?, ?)");
    query.bindValue(0, object.value(KEY_LOGIN).toInteger());
    query.bindValue(1, object.value(KEY_LOGIN).toString());
    query.bindValue(2, object.value(KEY_PASSWORD).toString());
    query.bindValue(3, object.value(KEY_DATE_REGISTRATION).toString());
    if (query.exec())
        return {
            {RESPONSE_TEXT, RESPONSE_CODE_OK}
        };

    return {
        {RESPONSE_TEXT, RESPONSE_CODE_BAD}
    };
}

QJsonObject Sql::addText()
{
    query.prepare("INSERT INTO texts (id, text, count_words) "
                  "VALUES (?, ?, ?)");
    query.bindValue(1, "eqweqw");
    query.bindValue(2, 1);
    if (query.exec())
        return {
            {RESPONSE_TEXT, RESPONSE_CODE_OK}
        };

    return {
        {RESPONSE_TEXT, RESPONSE_CODE_BAD}
    };
}

QJsonObject findText(QSqlQuery& query, const quint32 count_words) {
    query.prepare("SELECT * FROM texts "
                  "WHERE count_words = ?");
    query.bindValue(0, count_words);

    if (query.exec()) {
        uint16_t random_id = QRandomGenerator::global()->bounded(1, query.size());
        query.seek(random_id);

        return {
            {KEY_TEXT, query.value(1).toString()},
            {RESPONSE_TEXT, RESPONSE_CODE_OK}
        };
    }
    return {
        {RESPONSE_TEXT, RESPONSE_CODE_BAD}
    };
}

QJsonObject generateWords(QSqlQuery& query, const quint32 count_words) {
    if (query.exec("SELECT * FROM words")) {
        query.first();
        QStringList all_words = query.value(0).toString().split("\n");
        QString text;
        for (int i = 0; i < count_words; i++) {
            text += all_words[QRandomGenerator::global()->bounded(1, all_words.size())] + " ";
        }
        return {
            {KEY_TEXT, text},
            {RESPONSE_TEXT, RESPONSE_CODE_OK}
        };
    }
    return {
        {RESPONSE_TEXT, RESPONSE_CODE_BAD}
    };
}

const QJsonObject Sql::getRandomText(const TextType type, const quint32 count_words)
{
    try {
        switch(type) {
            case TextType::text:
                return findText(query, count_words);
            case TextType::words:
                return generateWords(query, count_words);
            }
    } catch (...) {
            return {
                {RESPONSE_TEXT, RESPONSE_CODE_BAD}
            };
    }
}
