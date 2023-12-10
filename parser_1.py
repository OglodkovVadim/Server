import json
from random import randint
import re
import os

def generateId():
    return randint(10000, 100000000)

def countWords(string):
    return len(string.split())

def deleteSpaces(text):
    return " ".join(text.split())

def deleteUpper(text):
    if (text.strip('.').strip(',').strip('!').strip('"').strip('?').isupper() and len(text) > 1):
        return ''
    return text 

def clearText(text):
    new_text = text.replace('\"', '').strip()

    for i in range(0, 10):
        if new_text[i] == new_text[i].lower():
            new_text = new_text[1:]
            i -= 1
        else :
            break
    return new_text.strip()

def writeToDict(id, Text, count, data, numberText):
    buff = {}
    buff["id"] = id
    buff["text"] = Text
    buff["count_words"] = count

    data[f"Text{numberText}"] = buff

def fileName(flag):
    if flag:
        return "ru_texts"
    return "en_texts"

def parsText(flag):
    partText = ""
    fullText = ""
    data = {}
    numberText = 0

    with open(f"{fileName(flag)}.txt", 'r', encoding="utf-8") as file:
        fullText = file.read()
        fullText = deleteSpaces(fullText)

    for j in range(10, 60, 10): 
        for i in fullText.split("."):
            if ("ГЛАВА" in i or "СЕРЖАНТ" in i or "ОСАДА" in i):
                continue
            if countWords(partText) + countWords(i) >= j:
                numberText += 1
                partText += i + '.'
                
                if (".." in partText):
                    partText = ""
                    continue
                if (countWords(clearText(partText)) == j):
                    writeToDict(generateId(), clearText(partText), countWords(clearText(partText)), data, numberText)
                partText = ""
            else:
                if (".." in partText):
                    partText = ""
                    continue
                partText += i + '.'

    if (flag):
        with open(f"{fileName(flag)}.json", "a", encoding="utf-8") as outfile:
            json.dump(data, outfile, indent=4, sort_keys=True,  ensure_ascii=False)
    else:
        with open(f"{fileName(flag)}.json", "a") as outfile:
            json.dump(data, outfile, indent=4, sort_keys=True,  ensure_ascii=False)

def main():
    os.remove("en_texts.json")
    os.remove("ru_texts.json")
    parsText(True)
    parsText(False)
    
    # with open ("text.txt", "r", encoding="utf-8") as outfile:
    #     str = outfile.read()
    # str = re.sub(r'[^\w\s]+|[\d]+', r'', str)
    # print(str.split())
    # # print('\n'.join(str.split()))

    print(len("But I wouldn't do that. Why, Huck, you'd to.".split()))

if __name__ == '__main__':
    main()