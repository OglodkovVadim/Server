#ifndef TEXT_H
#define TEXT_H

#include "libs.h"

class Text {
public:
    Text();
    Text(const uint16_t, const QString&, const uint16_t);
    Text(const QJsonObject&);

    const uint16_t getId() const;
    const QString& getText() const;
    const uint16_t getCountWords() const;

    void setId(const uint16_t);
    void setText(const QString&);
    void setCountWords(const uint16_t);

    const QJsonObject toJson() const;
    static void print(const QJsonObject& object);

private:
    uint16_t id;
    QString text;
    uint16_t count_words;
};

#endif // TEXT_H
