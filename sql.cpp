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
            "count_texts INT NOT NULL,"
            "user_id BIGINT NOT NULL,"
            "CONSTRAINT user_id_fk FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE"
            ")"
            );

        // create table for complited achivements (related to the table users by foreigen key user_id)
//        query.exec(
//            "CREATE TABLE IF NOT EXISTS complted_achivements ("
//            "id BIGINT PRIMARY KEY,"
//            "user_id BIGINT NOT NULL,"
//            "CONSTRAINT user_id_fk FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE"
//            ")"
//            );

        // create table for achivements (list of all achivements)
//        query.exec(
//            "CREATE TABLE IF NOT EXISTS achivements ("
//            "id BIGINT PRIMARY KEY,"
//            "title VARCHAR(64) NOT NULL,"
//            "text TEXT NOT NULL,"
//            "type INT NOT NULL"
//            ")"
//            );

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


const bool Sql::addUser(const QJsonObject& object)
{
    query.prepare("INSERT INTO users (id, login, password, date_registration) "
                  "VALUES (?, ?, ?, ?)"
                  );
    query.bindValue(0, object.value(KEY_ID).toString());
    query.bindValue(1, object.value(KEY_LOGIN).toString());
    query.bindValue(2, object.value(KEY_PASSWORD).toString());
    query.bindValue(3, object.value(KEY_DATE_REGISTRATION).toString());

    return query.exec();
}

const bool Sql::findUser(const QJsonObject& object)
{
    query.prepare("SELECT * FROM users "
                  "WHERE login = ? and password = ? "
                  );
    query.bindValue(0, object.value(KEY_LOGIN).toString());
    query.bindValue(1, object.value(KEY_PASSWORD).toString());

    return query.exec() && query.size() != 0;
}



const QJsonObject Sql::generateText(QSqlQuery& query, const quint32 count_words) {
    query.prepare("SELECT * FROM texts "
                  "WHERE count_words = ?");
    query.bindValue(0, count_words);

    if (query.exec()) {
        qDebug() <<  query.size();
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

const QJsonObject Sql::generateWords(QSqlQuery& query, const quint32 count_words) {
    if (query.exec("SELECT * FROM words")) {
        query.first();
        QStringList all_words = query.value(0).toString().split("\n");
        QString text;
        for (int i = 0; i < count_words; i++) {
            text += all_words[QRandomGenerator::global()->bounded(1, all_words.size())] + " ";
        }
        return {
            {KEY_WORDS, text},
            {RESPONSE_TEXT, RESPONSE_CODE_OK}
        };
    }
    return {
        {RESPONSE_TEXT, RESPONSE_CODE_BAD}
    };
}

const QJsonObject Sql::getRandomText(const TextType type, const quint32 count_words)
{
    return {
        {"text", "fsefgesgsgsgse"}
    };
//    try {
//        switch(type) {
//        case TextType::text:
//            return generateText(query, count_words);
//        case TextType::words:
//            return generateWords(query, count_words);
//        }
//    } catch (...) {
//        return {
//            {RESPONSE_TEXT, RESPONSE_CODE_BAD}
//        };
//    }
}

const bool Sql::addStatistic(const QJsonObject& object)
{
    query.prepare("SELECT * FROM statistics "
                  "WHERE user_id = ? ");
    query.bindValue(0, object.value(KEY_USER_ID).toString());

    if (query.exec()) {
        if (query.size() != 0) {
            query.next();
            int count = query.value(4).toInt();
            int max_speed = query.value(2).toInt();
            int average_speed = query.value(1).toInt();
            int average_count_mistakes = query.value(3).toInt();

            max_speed = std::max(max_speed, object.value(KEY_SPEED).toString().toInt());
            average_speed = (average_speed*count + object.value(KEY_SPEED).toString().toInt()) / (count + 1);
            average_count_mistakes = (average_count_mistakes*count + object.value(KEY_COUNT_MISTAKES).toString().toInt()) / (count + 1);
            ++count;

            qDebug() << "Updating";
            query.prepare("UPDATE statistics SET "
                          "average_speed = ?, max_speed = ?, average_count_mistakes = ?, count_texts = count_texts + 1 "
                          "WHERE user_id = ? ");

            query.bindValue(0, average_speed);
            query.bindValue(1, max_speed);
            query.bindValue(2, average_count_mistakes);
            query.bindValue(3, object.value(KEY_USER_ID).toString());
            query.exec();
        }
        else {
            qDebug() << "Inserting";
            query.prepare("INSERT INTO statistics (id, average_speed, max_speed, average_count_mistakes, count_texts, user_id) "
                          "VALUES (?, ?, ?, ?, ?, ?) ");

            query.bindValue(0, QRandomGenerator::global()->bounded(1, 1000000));
            query.bindValue(1, object.value(KEY_SPEED).toString());
            query.bindValue(2, object.value(KEY_SPEED).toString());
            query.bindValue(3, object.value(KEY_COUNT_MISTAKES).toString());
            query.bindValue(4, 1);
            query.bindValue(5, object.value(KEY_USER_ID).toString());
            query.exec();
        }
        return true;
    }
    return false;
}

const DangerousValues Sql::changeUsername(const QJsonObject& object)
{
    query.prepare("SELECT * FROM users "
                  "WHERE login = ? ");
    query.bindValue(0, object.value(KEY_LOGIN).toString());

    if (query.exec() && query.size() != 0)
        return DangerousValues::Incorrect;

    query.prepare("UPDATE users SET "
                  "login = ? "
                  "WHERE id = ? ");
    query.bindValue(0, object.value(KEY_LOGIN).toString());
    query.bindValue(1, object.value(KEY_USER_ID).toString());

    return query.exec() ? DangerousValues::True : DangerousValues::False;
}

const bool Sql::changePassword(const QJsonObject& object)
{
    query.prepare("UPDATE users SET "
                  "password = ? "
                  "WHERE id = ? ");
    query.bindValue(0, object.value(KEY_PASSWORD).toString());
    query.bindValue(1, object.value(KEY_USER_ID).toString());

    return query.exec();
}

const DangerousValues Sql::deleteAccount(const QJsonObject& object)
{
    query.prepare("SELECT * FROM users "
                  "WHERE id = ? and password = ? ");
    query.bindValue(0, object.value(KEY_USER_ID).toString());
    query.bindValue(1, object.value(KEY_PASSWORD).toString());

    if (query.exec() && query.size() != 0) {
        query.prepare("DELETE FROM users "
                      "WHERE id = ? ");
        query.bindValue(0, object.value(KEY_USER_ID).toString());
        return query.exec() ? DangerousValues::True : DangerousValues::False;
    }
    else
        return DangerousValues::Incorrect;
}


const QJsonObject Sql::getProfileStat(const uint32_t id)
{
    query.prepare("SELECT statistics.average_speed, statistics.max_speed, users.login, users.date_registration FROM users "
                  "JOIN statistics ON users.id = statistics.user_id and users.id = ? ");
    query.bindValue(0, id);

    if (query.exec()) {
        query.first();
        return {
            {KEY_LOGIN, query.value(2).toString()},
            {KEY_DATE_REGISTRATION, query.value(3).toString()},
            {KEY_AVERAGE_SPEED, query.value(0).toString()},
            {KEY_MAX_SPEED, query.value(1).toString()}
        };
    }

    return {};
}




//const QJsonObject Sql::addText()
//{
//    query.prepare("INSERT INTO texts (id, text, count_words) "
//                  "VALUES (?, ?, ?)");
//    query.bindValue(1, "eqweqw");
//    query.bindValue(2, 1);
//    if (query.exec())
//        return {
//            {RESPONSE_TEXT, RESPONSE_CODE_OK}
//        };

//    return {
//        {RESPONSE_TEXT, RESPONSE_CODE_BAD}
//    };
//}
