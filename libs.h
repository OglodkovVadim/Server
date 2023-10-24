#ifndef LIBS_H
#define LIBS_H

// for users
#define KEY_ID "id"
#define KEY_LOGIN "login"
#define KEY_PASSWORD "password"
#define KEY_DATE_REGISTRATION "date_registration"

// for texts
#define KEY_TEXT "text"
#define KEY_TYPE "type"
#define KEY_WORDS "words"
#define KEY_COUNT_WORDS "count_words"

// for achievements
#define KEY_TITLE "title"

#define KEY_USER_ID "user_id"

// for statistics
#define KEY_MAX_SPEED "max_speed"
#define KEY_AVERAGE_SPEED "average_speed"
#define KEY_AVERAGE_COUNT_MISTAKES "average_count_mistakes"

#define RESPONSE_TEXT "Response"
#define RESPONSE_CODE_BAD 503
#define RESPONSE_CODE_OK 200

#include <cstdint>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#endif // LIBS_H
