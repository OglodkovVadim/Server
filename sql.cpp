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
    try {
        data_base.setHostName(hostName);
        data_base.setUserName(userName);
        data_base.setPassword(password);
        data_base.setPort(port);
        data_base.open();
        qDebug() << "Data base opened sucessful";
    } catch (...) {
    qDebug() << data_base.lastError();
    }
}


// ???? This func will work only once
void Sql::createTables()
{
    try {
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
                "text TEXT NOT NULL,"
                "count_words INT NOT NULL"
            ")"
        );

    // create table for words (list of words)
        query.exec("CREATE TABLE words (text TEXT NOT NULL)");

    } catch (...) {
        qDebug() << data_base.lastError();
    }
}

void Sql::addUser(const QJsonObject& object)
{
    try {
        query.exec();
        query.prepare("INSERT INTO users (id, login, password, date_registration) "
                      "VALUES (?, ?, ?, ?)");
        query.bindValue(0, object.value(KEY_LOGIN).toInteger());
        query.bindValue(1, object.value(KEY_LOGIN).toString());
        query.bindValue(2, object.value(KEY_PASSWORD).toString());
        query.bindValue(3, object.value(KEY_DATE_REGISTRATION).toString());
        if (query.exec())
            qDebug() << "User added sucessful";
        else
            qDebug() << query.lastError();
    } catch (...) {
        qDebug() << data_base.lastError();
    }
}

void Sql::addText()
{
    query.prepare("INSERT INTO texts (id, text, count_words) "
                  "VALUES (?, ?, ?)");
    query.bindValue(1, "eqweqw");
    query.bindValue(2, 1);
    query.exec();
    qDebug() << "Text inserted sucessfully";
}

QJsonObject findText(QSqlQuery& query, const uint16_t _count_words) {
    query.prepare("SELECT * FROM texts "
                  "WHERE count_words = ?");
    query.bindValue(0, _count_words);

    if (query.exec()) {
        uint16_t random_id = QRandomGenerator::global()->bounded(1, query.size());
        query.seek(random_id);

        return {
            {KEY_TEXT, query.value(1).toString()},
            {KEY_RESPONSE, 200}
        };
    }
    return {
        {KEY_RESPONSE, 503}
    };
}

QJsonObject generateWords(QSqlQuery& query, const uint16_t _count_words) {
    query.exec("SELECT * FROM words");
    query.first();
    QStringList all_words = query.value(0).toString().split("\n");
    QString text;
    for (int i = 0; i < _count_words; i++) {
        text += all_words[QRandomGenerator::global()->bounded(1, all_words.size())] + " ";
    }
    return {
        {KEY_TEXT, text},
        {KEY_RESPONSE, 200}
    };
}

const QJsonObject Sql::getRandomText(TextType type, const uint16_t _count_words)
{
    try {
        switch(type) {
            case TextType::text:
                return findText(query, _count_words);
            case TextType::words:
                return generateWords(query, _count_words);
            }
    } catch (...) {
        qDebug() << data_base.lastError();
    }
    return {
        {KEY_RESPONSE, 503}
    };
}
