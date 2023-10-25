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
    Text(const quint32, const QString&, const quint32, const TextType);
    Text(const QJsonObject&);

    const quint32 getId() const;
    const QString& getText() const;
    const quint32 getCountWords() const;
    const TextType getTextType() const;

    void setId(const quint32);
    void setText(const QString&);
    void setCountWords(const quint32);
    void setTextType(const TextType);

    const QJsonObject toJson() const;
    static void print(const QJsonObject&);

    const static TextType stringToType(const QString&);
    const static QString typeToString(const TextType&);

private:
    quint32 id;
    QString text;
    quint32 count_words;
    TextType type;
};

#endif // TEXT_H
