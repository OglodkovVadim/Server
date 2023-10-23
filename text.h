#ifndef TEXT_H
#define TEXT_H

#include "libs.h"

enum class TextType {
    words,
    text
};

class Text {
public:
    Text();
    Text(const uint16_t, const QString&, const uint16_t, const TextType);
    Text(const QJsonObject&);

    const uint16_t getId() const;
    const QString& getText() const;
    const uint16_t getCountWords() const;
    const TextType getTextType() const;

    void setId(const uint16_t);
    void setText(const QString&);
    void setCountWords(const uint16_t);
    void setTextType(const TextType);

    const QJsonObject toJson() const;
    static void print(const QJsonObject&);

    const static TextType stringToType(const QString&);
    const static QString typeToString(const TextType&);

private:
    uint16_t id;
    QString text;
    uint16_t count_words;
    TextType type;
};

#endif // TEXT_H
