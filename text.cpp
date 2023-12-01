#include "text.h"

Text::Text():
    id(0),
    text(""),
    count_words(0)
{}

Text::Text(const quint32 _id, const QString& _text, const quint32 _count_words, const TextType _type) :
    id(_id),
    text(_text),
    count_words(_count_words),
    type(_type)
{}

Text::Text(const QJsonObject& obj) :
    id(obj.value(KEY_ID).toInt()),
    text(obj.value(KEY_TEXT).toString()),
    count_words(obj.value(KEY_COUNT_WORDS).toInt()),
    type(Text::stringToType(obj.value(KEY_TYPE).toString()))
{}

const TextType Text::stringToType(const QString& str) {
    return str == KEY_TEXT ? TextType::text : TextType::words;
}

const QString Text::typeToString(const TextType& type) {
    return type == TextType::text ? KEY_TEXT : KEY_WORDS;
}

const quint32 Text::getId() const { return id; }
const QString& Text::getText() const { return text; }
const quint32 Text::getCountWords() const { return count_words; }

void Text::setId(const quint32 _id) { this->id = _id; }
void Text::setText(const QString &_text) { this->text = _text; }
void Text::setCountWords(const quint32 _countWords) { this->count_words = _countWords; }

void Text::print(const QJsonObject& object) {
    qDebug() << " id: " << object.value(KEY_ID).toInt() << "\n"
             << "text: " << object.value(KEY_TEXT).toString() << "\n"
             << "count_words: " << object.value(KEY_COUNT_WORDS).toInt();
}

const QJsonObject Text::toJson() const {
    QJsonObject obj;
    obj.insert(KEY_ID, static_cast<int>(this->id));
    obj.insert(KEY_TEXT, this->text);
    obj.insert(KEY_COUNT_WORDS, static_cast<int>(this->count_words));
    obj.insert(KEY_TYPE, Text::typeToString(this->type));
    return obj;
}
