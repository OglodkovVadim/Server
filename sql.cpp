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

        // create table for texts (list of all texts)
        query.exec(
            "CREATE TABLE IF NOT EXISTS ru_texts ("
            "id BIGINT PRIMARY KEY,"
            "text TEXT NOT NULL,"
            "count_words INT NOT NULL"
            ")"
            );

        query.exec(
            "CREATE TABLE IF NOT EXISTS en_texts ("
            "id BIGINT PRIMARY KEY,"
            "text TEXT NOT NULL,"
            "count_words INT NOT NULL"
            ")"
            );

        // create table for words (list of words)
        query.exec("CREATE TABLE IF NOT EXISTS ru_words (text TEXT NOT NULL)");
        query.exec("CREATE TABLE IF NOT EXISTS en_words (text TEXT NOT NULL)");

    } catch (...) {
        qDebug() << sql_database.lastError();
    }
}



const Auth Sql::addUser(const QJsonObject& object)
{
    Auth auth;
    try {
        query.prepare("SELECT * FROM users "
                      "WHERE login = ? ");
        query.bindValue(0, object.value(KEY_LOGIN).toString());
        query.exec();
        if (query.size() != 0) {
            auth.bool_values = BoolValues::Incorrect;
            return auth;
        }

        auth.id = QRandomGenerator::global()->bounded(10000, 10000000);
        query.prepare("INSERT INTO users (id, login, password, date_registration) "
                      "VALUES (?, ?, ?, ?) ");
        query.bindValue(0, auth.id);
        query.bindValue(1, object.value(KEY_LOGIN).toString());
        query.bindValue(2, object.value(KEY_PASSWORD).toString());
        query.bindValue(3, QDateTime::currentDateTime().toString("dd.MM.yyyy"));
        query.exec();

        auth.bool_values = BoolValues::True;
        return auth;

    } catch(...) {
        qDebug() << query.lastError();
        return auth;
    }
}

const Auth Sql::findUser(const QJsonObject& object)
{
    Auth auth;
    try {
        query.prepare("SELECT * FROM users "
                      "WHERE login = ? and password = ? ");
        query.bindValue(0, object.value(KEY_LOGIN).toString());
        query.bindValue(1, object.value(KEY_PASSWORD).toString());
        query.exec();
        if (query.size() != 0) {
            query.first();
            auth.bool_values = BoolValues::True;
            auth.id = query.value(0).toString().toInt();
            return auth;
        }
        auth.bool_values = BoolValues::Incorrect;
        return auth;
    } catch(...) {
        qDebug() << query.lastError();
        return auth;
    }

}


const QJsonObject Sql::generateText(const Language lang, const quint32 count_words) {
    try {
        switch(lang) {
        case Language::ru:
            query.prepare("SELECT * FROM ru_texts "
                          "WHERE count_words = ?");
            break;
        default:
            query.prepare("SELECT * FROM en_texts "
                          "WHERE count_words = ?");
            break;
        }

        query.bindValue(0, count_words);
        query.exec();

        uint16_t random_id = QRandomGenerator::global()->bounded(1, query.size());
        query.seek(random_id);

        return {
            {KEY_TEXT, query.value(1).toString()}
        };


    } catch(...) {
        qDebug() << query.lastError();
        return {};
    }
}

const QJsonObject Sql::generateWords(const Language lang, const quint32 count_words) {
    try {
        switch(lang) {
        case Language::ru:
            query.exec("SELECT * FROM ru_words ");
            break;
        default:
            query.exec("SELECT * FROM en_words ");
            break;
        }
        query.first();
        QStringList all_words = query.value(0).toString().split("\n");
        QString text;
        for (int i = 0; i < count_words; i++) {
            text += all_words[QRandomGenerator::global()->bounded(1, all_words.size())] + " ";
        }
        text.replace("\r", "");
        return {
            {KEY_WORDS, text.sliced(0, text.size()-1)}
        };

    } catch(...) {
        throw query.lastError().text();
        return {};
    }
}

const QJsonObject Sql::getRandomText(const Language lang, const TextType type, const quint32 count_words)
{
    try {
        switch(type) {
        case TextType::text:
            return generateText(lang, count_words);
        case TextType::words:
            return generateWords(lang, count_words);
        }
    }
    catch(std::exception e) {
        qDebug() << e.what();
        return {};
    }
}



const BoolValues Sql::changeUsername(const QJsonObject& object)
{
    try {
        query.prepare("SELECT * FROM users "
                      "WHERE login = ?");
        query.bindValue(0, object.value(KEY_LOGIN).toString());
        query.exec();

        if (query.size() != 0)
            return BoolValues::Incorrect;

        query.prepare("SELECT * FROM users "
                      "WHERE id = ? and password = ?");
        query.bindValue(0, object.value(KEY_USER_ID).toString());
        query.bindValue(1, object.value(KEY_PASSWORD).toString());
        query.exec();

        if (query.size() == 0)
            return BoolValues::Incorrect;

        query.prepare("UPDATE users SET "
                      "login = ? "
                      "WHERE id = ? ");
        query.bindValue(0, object.value(KEY_LOGIN).toString());
        query.bindValue(1, object.value(KEY_USER_ID).toString());

        query.exec();
        return BoolValues::True;

    } catch (...) {
        qDebug() << query.lastError();
        return BoolValues::False;
    }

}

const BoolValues Sql::changePassword(const QJsonObject& object)
{
    try {
        query.prepare("SELECT * FROM users "
                      "WHERE password = ? and id = ? ");
        query.bindValue(0, object.value(KEY_OLD_PASSWORD).toString());
        query.bindValue(1, object.value(KEY_USER_ID).toString());
        query.exec();

        if (query.size() == 0)
            return BoolValues::Incorrect;

        query.prepare("UPDATE users SET "
                      "password = ? "
                      "WHERE id = ? ");
        query.bindValue(0, object.value(KEY_NEW_PASSWORD).toString());
        query.bindValue(1, object.value(KEY_USER_ID).toString());

        query.exec();
        return BoolValues::True;

    } catch (...) {
        qDebug() << query.lastError();
        return BoolValues::False;
    }

}

const BoolValues Sql::deleteAccount(const QJsonObject& object)
{
    try {
        query.prepare("SELECT * FROM users "
                      "WHERE id = ? and password = ? ");
        query.bindValue(0, object.value(KEY_USER_ID).toString());
        query.bindValue(1, object.value(KEY_PASSWORD).toString());
        query.exec();

        if (query.size() != 0) {
            query.prepare("DELETE FROM users "
                          "WHERE id = ? ");
            query.bindValue(0, object.value(KEY_USER_ID).toString());
            return query.exec() ? BoolValues::True : BoolValues::False;
        }
        else
            return BoolValues::Incorrect;
    }
    catch(...) {
        qDebug() << query.lastError();
        return BoolValues::False;
    }
}


const bool Sql::addStatistic(const QJsonObject& object)
{
    try {
        query.prepare("SELECT * FROM statistics "
                      "WHERE user_id = ? ");
        query.bindValue(0, object.value(KEY_USER_ID).toString());
        query.exec();

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
            return query.exec();
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
            return query.exec();
        }

    } catch(...) {
        qDebug() << query.lastError();
        return false;
    }

}

const QJsonObject Sql::getProfileStat(const uint32_t id)
{
    try {
        query.prepare("SELECT statistics.average_speed, statistics.max_speed, users.login, users.date_registration FROM users "
                        "LEFT OUTER JOIN statistics ON users.id = statistics.user_id "
                        "WHERE users.id = ? ");
        qDebug() << id;
        query.bindValue(0, id);
        query.exec();

        if (query.size() != 0) {
            query.first();
            return {
                {KEY_LOGIN, query.value(2).toString()},
                {KEY_DATE_REGISTRATION, query.value(3).toString()},
                {KEY_AVERAGE_SPEED, query.value(0).toString()},
                {KEY_MAX_SPEED, query.value(1).toString()}
            };
        }
        return {};

    } catch(...) {
        qDebug() << query.lastError();
        return {};
    }
}

const void Sql::addText(const Language lang)
{
    QFile file;
    QString text;
    QString ru_en_texts;

    switch(lang) {
    case Language::ru:
        ru_en_texts = "ru_texts";
        break;
    default:
        ru_en_texts = "en_texts";
        break;
    }

    query.exec("DELETE FROM " + ru_en_texts);
    file.setFileName(":/data/" + ru_en_texts + ".json");
    file.open(QIODevice::ReadOnly);
    text = file.readAll();
    file.close();
    for (int i = 0; i < 10000; i++) {
        auto obj = QJsonDocument::fromJson(text.toUtf8()).object().value("Text"+QString::number(i)).toObject();
        if (!obj.isEmpty()) {
            query.prepare("INSERT INTO " + ru_en_texts + " (id, text, count_words) "
                          "VALUES(?, ?, ?) ");
            query.bindValue(0, obj.value(KEY_ID).toInt());
            query.bindValue(1, obj.value(KEY_TEXT).toString());
            query.bindValue(2, obj.value(KEY_COUNT_WORDS).toInt());
            query.exec();
        }

    }
}

const void Sql::addWords(const Language lang)
{
    QFile file;
    QString text;
    QString ru_en_words;

    switch(lang) {
    case Language::ru:
        ru_en_words = "ru_words";
        break;
    default:
        ru_en_words = "en_words";
        break;
    }

    query.exec("DELETE FROM " + ru_en_words);
    file.setFileName(":/data/" + ru_en_words + ".txt");
    file.open(QIODevice::ReadOnly);
    text = file.readAll();
    file.close();

    query.prepare("INSERT INTO " + ru_en_words + " (text) "
                                                 "VALUES(?) ");
    query.bindValue(0, text);
    query.exec();
}
