
# 1) counter (счетчик) - переменная отвечающая за кол-во слов, чтобы понимать когда начать искать ближайщую точку
# 2) text (текст) - переменная отвечающя за сам текст (уже отрезанный)
# 3) id (номер) - идентификационный номер куска отрезанного текста

# 1) generateId - функция, которая генерирует уникальный id
# 2) writeToJson - функция, записывающая данные в json файл
# Циклы
# 1) while условие:
# 2) for переменная in значения:
# While
# a = 0
# while a <= 4:
# a += 1
# print(a)
# for
# list = [125, 124]
# list.append(12)
# list.append(56)
# #
# for i in list:
# print(i, ' - ', i*i)
# while True:
# бесконечный
# while - просто какой-то (может быть и бесконеычый) цикл с условием
# for - цикл для прохода по списку значений

import json
from random import randint

def generateId():
    return randint(10000, 100000000)

def countWords(string):
    return string.count(" ") + 1

def writeToDict(id, Text, count, data, numberText):
    buff = {}
    buff["id"] = id
    buff["Text"] = Text
    buff["count"] = count

    data[f"Text{numberText}"] = buff

def main():
    partText = ""
    fullText = ""
    data = {}
    numberText = 0

    with open("Data/text.txt", 'r', encoding="utf-8") as file:
        fullText = file.read()

    for i in fullText.split("."):
        if countWords(partText) + countWords(i) >= 50:
            numberText += 1
            partText += i + '.'
            writeToDict(generateId(), partText.replace('\n', ' '), countWords(partText), data, numberText)
            partText = ""
        else:
            partText += i + '.'

    with open("Data/parsText.json", "w", encoding="utf-8") as outfile:
        json.dump(data, outfile, indent=4, sort_keys=True,  ensure_ascii=False)

if __name__ == '__main__':
    main()
