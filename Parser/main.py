import json
from random import randint

def writeToDict(id, value, count, dict):
    data = {}
    data['id'] = id
    data['value'] = value
    data['count'] = count

    dict[f'Text{randint(1, 10000)}'] = data

def countWords(text):
     return len(text.split())

def printDict(dict):
    for key, val in dict.items():
        print(key)
        for key2, val2 in val.items():
            print(key2, ': ', val2)

a = ('PyCharm — это мощная интегрированная среда разработки для языка программирования Python, созданная компанией JetBrains. '
     'Одной из важных возможностей PyCharm является интеграция с системами контроля версий, такими как Git и GitHub.'
     'Регулярные выражения — это формальный язык поиска и манипулирования подстроками в тексте. Они используются для поиска определенных паттернов в строке, таких как слова, числа, электронные адреса и т.д.'
     'Для подсчета количества слов в строке с помощью регулярных выражений, мы можем использовать функцию findall() из модуля re. Эта функция ищет все совпадения с регулярным выражением и возвращает список найденных строк.')

partText = ''

dict = {}

for i in a.split('.'):
     if countWords(i) + countWords(partText) >= 20:
          partText += i
          writeToDict(randint(100000, 9999999), partText, countWords(partText), dict)
          partText = ''
     else:
          partText += i

printDict(dict)

with open('data.json', 'w', encoding='utf-8') as file:
    json.dump(dict, file, ensure_ascii=False)






# def peremnojit(id, value, count, numberText, numberLetters):
#
#     data[f'Text{numberText}'] = buff
#     data['number'] = numberLetters
#     return num1*num2



# def main():
#     # 2 * 3
#     # 5 * 7
#     # 6 * 8
#     # 9 * 7
#     data = {}
#     numberText = 1
#     for ////
#         numberText += 1
#         writeDict(12, partText, countWords(partText), data, numberText)
#
#
#



