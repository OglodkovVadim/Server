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

using namespace Qt::StringLiterals;

class UserManager {
public:
    UserManager() = default;

    const QString signIn();
    const QString signUp();
    const QString deleteAcc();
};



class Text {
public:
    Text()
        : id(0),  text(""), countWords(0)
    {}
    Text(uint16_t _id, QString& _text, uint16_t _countWords)
        : id(_id), text(_text), countWords(_countWords)
    {}

    uint16_t
    QString

private:
    uint16_t id;
    QString text;
    uint16_t countWords;
};

class User {
public:
    User() : {};
private:
    uint16_t id;
    QString login;
    QString password;
//    QVector<uint16_t>* ;
};































int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QHttpServer httpServer;
    QSqlDatabase* db;
    QSqlQuery query;

    db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
    db->setHostName("127.0.0.1");
    db->setUserName("postgres");
    db->setPassword("1234");
    db->setPort(5432);

    if (!db->open())
        qDebug() << db->lastError();
    else {
        qDebug() << "data base opened sucessful";
        query = QSqlQuery(*db);
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS UsersServer (Id TEXT, Login TEXT, Password TEXT)")) {
        qDebug() << query.lastError();
    }

    if (httpServer.listen(QHostAddress::Any, 80)) {
        qDebug() << "Listen...";
    }

    httpServer.route("/", []() {
        return "Hi";
    });

    httpServer.route("/resp/", [](const QHttpServerRequest &request) {
        QJsonDocument json;
        if (request.method() == QHttpServerRequest::Method::Get) {
            QJsonArray recordsArray;
            QSqlQuery query("SELECT * FROM UsersServer");

            if (query.size() > 0)
                while(query.next()) {
                    QJsonObject obj;
                    obj.insert("id", query.value(0).toString());
                    obj.insert("login", query.value(1).toString());
                    obj.insert("password", query.value(2).toString());
                    recordsArray.push_back(obj);
                }
            else
                qDebug() << "There are not users";

            json.setArray(recordsArray);
        }
        return json.toJson();
    });

    httpServer.route("/auth/", [](const QHttpServerRequest& request) {
        QString messageText = "Faild";
        auto status = QHttpServerResponse::StatusCode::BadRequest;
        if (request.method() == QHttpServerRequest::Method::Post) {
            QSqlQuery query;
            query.prepare("INSERT INTO UsersServer (Id, Login, Password) "
                          "VALUES (:id, :login, :password)");
            query.bindValue(":id", QJsonDocument::fromJson(request.body()).object().value("id").toString());
            query.bindValue(":login", QJsonDocument::fromJson(request.body()).object().value("login").toString());
            query.bindValue(":password", QJsonDocument::fromJson(request.body()).object().value("password").toString());
            if (query.exec()) {
                messageText = "Success";
                status = QHttpServerResponse::StatusCode::Ok;
                qDebug() << "Inserting complite";
            }
            else
                messageText = query.lastError().text();
        }
        return QHttpServerResponse {
            QJsonObject {
                {"message", messageText}
            },
            status
        };
    });

// return {
//      { " message ": " error/success"}
//    }


    httpServer.route("/del", [] () {
        QSqlQuery query("DELETE from UsersServer");
        return "Deleted data :(";
    });


    // test git security

    return app.exec();
}
