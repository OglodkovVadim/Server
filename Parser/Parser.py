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

def deleteSpaces(text):
    return ' '.join(text.split())

def main():
    partText = ""
    fullText = ""
    data = {}
    numberText = 0

    with open("data.txt", 'r') as file:
        fullText = file.read()

    for i in fullText.split("."):
        if countWords(partText) + countWords(i) >= 50:
            numberText += 1
            partText += i + '.'
            writeToDict(generateId(), deleteSpaces(partText.replace('\n', ' ')), countWords(partText), data, numberText)
            partText = ""
        else:
            partText += i + '.'

    with open("parsText.json", "w", encoding="utf-8") as outfile:
        json.dump(data, outfile, indent=4, sort_keys=True,  ensure_ascii=False)

if __name__ == '__main__':
    main()
