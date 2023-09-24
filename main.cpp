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
        return "Hello world";
    });

    httpServer.route("/auth/", [](const QHttpServerRequest &request) {
        QJsonDocument json;
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
        return json.toJson();
    });

//    httpServer.route("/auth/", []() {
//        QSqlQuery query("INSERT INTO UsersServer (Id, Login, Password) "
//                        "VALUES ('alexey', 'shabarshov', 'alex228')");
//        qDebug() << "Inserting complite";
//        return "Auth Success";
//    });

//    httpServer.route("/resp", [] () {
//        QSqlQuery query("SELECT * from UsersServer");
//        if (query.size() > 0)
//            while(query.next()) {
//                qDebug() << query.value(0).toString();
//                qDebug() << query.value(1).toString();
//                qDebug() << query.value(2).toString();
//            }
//        else
//            qDebug() << "There are not users";

//        return "Get data";
//    });

    httpServer.route("/del", [] () {
        QSqlQuery query("DELETE from UsersServer");

        return "Deleted data :(";
    });

    return app.exec();
}
