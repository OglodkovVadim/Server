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

#include "sql.h"
#include "text.h"

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




































int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

//    QHttpServer httpServer;

    Sql sql;
    sql.init(hostName, userName, password, port);
//    sql.createTables();
//    QJsonObject obj;
//    obj.insert(KEY_ID, 123459);
//    obj.insert(KEY_LOGIN, "dada");
//    obj.insert(KEY_PASSWORD, "dad");
//    obj.insert(KEY_DATE_REGISTRATION, QDate(2000, 12, 2).toString("yyyy.MM.dd"));
//    sql.addUser(obj);
//    sql.addText();
    while (true)
        if (sql.getRandomText(1).value(KEY_ID) == 0)
            Text::print(sql.getRandomText(1));

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

    return app.exec();
}
