#ifndef USER_H
#define USER_H

#include "qdatetime.h"
#include "libs.h"

class User
{
public:
    explicit User();
    explicit User(const QJsonObject& obj);
    User (
        const uint16_t,
        const QString,
        const QString,
        const QDate
    );

    const uint16_t getId() const;
    const QString getLogin() const;
    const QString getPassword() const;
    const QDate getDateRegistration() const;

    void setId(const uint16_t);
    void setLogin(const QString);
    void setPassword(const QString);
    void setDate(const QDate);

    const QJsonObject toJson();
//    const QDate stringToDate(QString) noexcept;

private:
    uint16_t id;
    QString login;
    QString password;
    QDate date_registration;
};

#endif // USER_H
