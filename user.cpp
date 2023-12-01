#include "user.h"
#include "libs.h"

User::User() :
    id(0),
    login(""),
    password(""),
    date_registration(QDate())
{}

User::User(
    const quint32 id,
    const QString login,
    const QString password,
    const QDate date_registration
) :
    id(id),
    login(login),
    password(password),
    date_registration(date_registration)

{}

const quint32 User::getId() const {
    return this->id;
}
const QString User::getLogin() const {
    return this->login;
}
const QString User::getPassword() const {
    return this->password;
}
const QDate User::getDateRegistration() const {
    return this->date_registration;
}

void User::setId(const quint32 id) {
    this->id = id;
}
void User::setLogin(const QString login) {
    this->login = login;
}
void User::setPassword(const QString password) {
    this->password = password;
}
void User::setDate(const QDate date_registration) {
    this->date_registration = date_registration;
}

//const QDate stringToDate(QString string) noexcept
//{
//    if (!string.isEmpty()) {
//        auto listDate = string.split(".");
//        return QDate(listDate[0].toInt(), listDate[1].toInt(), listDate[2].toInt());
//    }
//    return QDate();
//}

User::User(const QJsonObject& object) :
    id(object.value(KEY_ID).toInt()),
    login(object.value(KEY_LOGIN).toString()),
    password()//,
//    date_registration(QDate(stringToDate(object.value(KEY_DATE_REGISTRATION).toString())))
{}

const QJsonObject User::toJson()
{
    QJsonObject obj;
    obj.insert(KEY_ID, static_cast<int>(this->id));
    obj.insert(KEY_LOGIN, this->login);
    obj.insert(KEY_PASSWORD, this->password);
    obj.insert(KEY_DATE_REGISTRATION, this->date_registration.toString());
    return obj;
}
















