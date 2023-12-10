#ifndef LIBS_H
#define LIBS_H

// for users
#define KEY_ID "id"
#define KEY_LOGIN "login"
#define KEY_PASSWORD "password"
#define KEY_OLD_PASSWORD "old_password"
#define KEY_NEW_PASSWORD "new_password"
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
#define KEY_SPEED "speed"
#define KEY_MAX_SPEED "max_speed"
#define KEY_COUNT_TEXTS "count_texts"
#define KEY_AVERAGE_SPEED "average_speed"
#define KEY_COUNT_MISTAKES "count_mistakes"
#define KEY_AVERAGE_COUNT_MISTAKES "average_count_mistakes"

#define RESPONSE_MESSAGE "Message"
#define RESPONSE_CODE_BAD 503
#define RESPONSE_CODE_OK 200

#include <cstdint>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

enum class BoolValues {
    True,
    False,
    Incorrect
};

enum class Language {
    ru,
    en
};


struct Auth {
    BoolValues bool_values = BoolValues::False;
    uint32_t id = 0;

    Auth operator= (const Auth& other) noexcept {
        this->id = other.id;
        this->bool_values = other.bool_values;
        return *this;
    }
};



#endif // LIBS_H
