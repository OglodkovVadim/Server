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
        const quint32,
        const QString,
        const QString,
        const QDate
    );

    const quint32 getId() const;
    const QString getLogin() const;
    const QString getPassword() const;
    const QDate getDateRegistration() const;

    void setId(const quint32);
    void setLogin(const QString);
    void setPassword(const QString);
    void setDate(const QDate);

    const QJsonObject toJson();
//    const QDate stringToDate(QString) noexcept;

//    const User getUserFromDB(const quint32 id);

private:
    quint32 id;
    QString login;
    QString password;
    QDate date_registration;
};

#endif // USER_H
