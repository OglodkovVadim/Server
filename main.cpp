#include <QHttpServer>
#include <QHttpServerResponse>
#include <QtHttpServerDepends>

#include <QCoreApplication>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVector>

#define KEY_ID "id"
#define KEY_TEXT "text"
#define KEY_COUNT "countWords"
#define KEY_LOGIN "login"
#define KEY_PASSWORD "password"
#define KEY_SOLVED_TEXTS "solvedTexts"

#include "sql.h"
#include "server.h"

const QString hostName = "127.0.0.1";
const QString userName = "postgres";
const QString password = "1234";
const uint16_t port = 5432;


using namespace Qt::StringLiterals;

class UserManager {
public:
    UserManager() = default;

    const QString signIn();
    const QString signUp();
    const QString deleteAcc();
};


/*
 *
 * Achivement
 *
 */


//class Text {
//public:
//    Text() :
//        id(0),
//        text(""),
//        countWords(0)
//    {}
//    Text(const uint16_t _id, const QString& _text, const uint16_t _countWords) :
//        id(_id),
//        text(_text),
//        countWords(_countWords)
//    {}
//    Text(const QJsonObject& obj) :
//        id(obj.value(KEY_ID).toInt()),
//        text(obj.value(KEY_TEXT).toString()),
//        countWords(obj.value(KEY_COUNT).toInt())
//    {}

//    const uint16_t getId() { return id; }
//    const QString& getText() { return text; }
//    const uint16_t getCountWords() { return countWords; }

//    void setId(uint16_t _id) { this->id = _id; }
//    void setText(QString& _text) { this->text = _text; }
//    void setCountWords(uint16_t _countWords) { this->countWords = _countWords; }

//    const QJsonObject toJson() const {
//        QJsonObject obj;
//        obj.insert(KEY_ID, this->id);
//        obj.insert(KEY_TEXT, this->text);
//        obj.insert(KEY_COUNT, this->countWords);
//        return obj;
//    }

//private:
//    uint16_t id;
//    QString text;
//    uint16_t countWords;
//};

//class User {
//public:
//    User() :
//        id(0),
//        login(""),
//        password(""),
//        texts(0)
//    {}
//    User(const uint16_t _id, const QString& _login, const QString& _password, const QVector<Text *>& _texts) :
//        id(_id),
//        login(_login),
//        password(_password),
//        texts(_texts)
//    {}
//    User(const QJsonObject& obj) :
//        id(obj.value(KEY_ID).toInt()),
//        login(obj.value(KEY_LOGIN).toString()),
//        password(obj.value(KEY_PASSWORD).toString()),
//        texts(obj.value(KEY_SOLVED_TEXTS).toArray())

//    {}
//private:
//    uint16_t id;
//    QString login;
//    QString password;
//    QVector<Text *> texts;
//    // e-mail
//};






























int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QHttpServer httpServer;
    QSqlDatabase* db;
    QSqlQuery query;
    Server server;
    Sql sql;

    sql.init(hostName, userName, password, port);
    server.init();

    server.routeHome(sql);
    server.routeSignIn(sql);
    server.routeSignUp(sql);
    server.routeProfile(sql);
    server.routeSettings(sql);
    server.routeStatistic(sql);


//    qDebug() << sizeof(int) << Qt::endl << sizeof(QSqlDatabase);


    httpServer.route("/", []() {
        return "Hi";
    });
//    int* arr = new int[4];
//    qDebug() << "arr: " << arr;
//    qDebug() << "&arr[0]: " << &arr[0];
//    qDebug() << "&arr[1]: " << &arr[1];
//    qDebug() << "&arr[2]: " << &arr[2];
//    qDebug() << "&arr[3]: " << &arr[3];

//    QHttpServer httpServer;
//    QSqlDatabase* db;
//    QSqlQuery query;

//    db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
//    db->setHostName("127.0.0.1");
//    db->setUserName("postgres");
//    db->setPassword("1234");
//    db->setPort(5432);

//    json.setArray(recordsArray);
//        }
//        return json.toJson();
//    });

//    httpServer.route("/auth/", [](const QHttpServerRequest& request) {
//        QString messageText = "Faild";
//        auto status = QHttpServerResponse::StatusCode::BadRequest;
//        if (request.method() == QHttpServerRequest::Method::Post) {
//            QSqlQuery query;
//            query.prepare("INSERT INTO UsersServer (Id, Login, Password) "
//                          "VALUES (:id, :login, :password)");
//            query.bindValue(":id", QJsonDocument::fromJson(request.body()).object().value("id").toString());
//            query.bindValue(":login", QJsonDocument::fromJson(request.body()).object().value("login").toString());
//            query.bindValue(":password", QJsonDocument::fromJson(request.body()).object().value("password").toString());
//            if (query.exec()) {
//                messageText = "Success";
//                status = QHttpServerResponse::StatusCode::Ok;
//                qDebug() << "Inserting complite";
//            }
//            else
//                messageText = query.lastError().text();
//        }
//        return QHttpServerResponse {
//            QJsonObject {
//                {"message", messageText}
//            },
//            status
//        };
//    });

// return {
//      { " message ": " error/success"}
//    if (!db->open())
//        qDebug() << db->lastError();
//    else {
//        qDebug() << "data base opened sucessful";
//        query = QSqlQuery(*db);
//    }

//    if (httpServer.listen(QHostAddress::Any, 80)) {
//        qDebug() << "Listen...";
//    }

//    httpServer.route("/", []() {
//        return "Hi";
//    });

//    httpServer.route("/<arg>/<arg>/",
//                    QHttpServerRequest::Method::Get,
//                    [](const QString &firstArg,
//                           const QString &secondArg,
//                           const QHttpServerRequest &request) {
//                        qDebug() << firstArg << " " << secondArg;
//                        return "Ok";
//                    });





//    Sql sql;
//    sql.init(hostName, userName, password, port);
//    Server server;
//    qDebug() << server.init();
   // server.routeHome();


//    QHttpServer httpServer;

//    sql.createTables();
//    sql.addText();
//    QJsonObject obj;
//    obj.insert(KEY_ID, 123459);
//    obj.insert(KEY_LOGIN, "dada");
//    obj.insert(KEY_PASSWORD, "dad");
//    obj.insert(KEY_DATE_REGISTRATION, QDate(2000, 12, 2).toString("yyyy.MM.dd"));
//    sql.addUser(obj);
//    sql.addText();

//    qDebug() << sql.getRandomText(TextType::words, 20);
//    qDebug() << sql.getRandomText(TextType::text, 1);

//    if (httpServer.listen(QHostAddress::Any, 80)) {
//        qDebug() << "Listen...";
//    }

//    httpServer.route("/", []() {
//        return "Hi";
//    });

//    httpServer.route("/resp/", [](const QHttpServerRequest &request) {
//        QJsonDocument json;
//        if (request.method() == QHttpServerRequest::Method::Get) {
//            QJsonArray recordsArray;
//            QSqlQuery query("SELECT * FROM UsersServer");

//            if (query.size() > 0)
//                while(query.next()) {
//                    QJsonObject obj;
//                    obj.insert("id", query.value(0).toString());
//                    obj.insert("login", query.value(1).toString());
//                    obj.insert("password", query.value(2).toString());
//                    recordsArray.push_back(obj);
//                }
//            else
//                qDebug() << "There are not users";

//            json.setArray(recordsArray);
//        }
//        return json.toJson();
//    });

//    httpServer.route("/auth/", [&sql] (const QHttpServerRequest& request)  {
//        sql.addUser(QJsonDocument::fromJson(request.body()).object());
//    });


//    httpServer.route("/del", [] () {
//        QSqlQuery query("DELETE from UsersServer");
//        return "Deleted data :(";
//    });


    // test git security


    // test git security

    return app.exec();
}
