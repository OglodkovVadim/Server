#ifndef USER_H
#define USER_H

#define KEY_ID "id"
#define KEY_LOGIN "login"
#define KEY_PASSWORD "password"
#define KEY_DATE_REGISTRATION "date_registration"

#include "qdatetime.h"
#include <cstdint>
#include <QString>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class User
{
public:
    User();
    User (
        const uint16_t,
        const QString,
        const QString,
        const QDate
        );
    User(const QJsonObject& obj);

    const uint16_t getId() const { return this->id; }
    const QString getLogin() const { return this->login; }
    const QString getPassword() const { return this->password; }
    const QDate getDateRegistration() const { return this->date_registration; }

    void setId(const uint32_t id) { this->id = id; }
    void setLogin(const QString login) { this->login = login; }
    void setPassword(const QString password) { this->password = password; }
    void setDate(const QDate date_registration) { this->date_registration = date_registration; }

    const QJsonObject toJson();

private:
    uint32_t id;
    QString login;
    QString password;
    QDate date_registration;
};

#endif // USER_H
