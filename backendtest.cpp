#include "backendtest.h"
#include "qtestcase.h"

BackendTest::BackendTest(QObject *parent)
    : QObject{parent}
{
    server.init();
    sql.init();
}

void BackendTest::test_getTypeText() // 1 / 1
{
    QCOMPARE(server.getTextType("text"), TextType::text);
    QCOMPARE(server.getTextType("words"), TextType::words);
}

void BackendTest::test_getLanguage() // 1 / 1
{
    QCOMPARE(server.getLanguage("ru"), Language::ru);
    QCOMPARE(server.getLanguage("en"), Language::en);
}

void BackendTest::test_addUser()// 26 / 34
{
    // add not exist user
    QCOMPARE(sql.addUser(QJsonObject{
                {KEY_LOGIN, "vadim"},
                {KEY_PASSWORD, "password"}
             }).bool_values, BoolValues::True);

    // add exist user
    QCOMPARE(sql.addUser(QJsonObject{
                {KEY_LOGIN, "exist_user"},
                {KEY_PASSWORD, "123"}
             }).bool_values, BoolValues::Incorrect);
}

void BackendTest::test_findUser() // 15 / 21
{
    // find exist user with correct login and password
    QCOMPARE(sql.findUser(QJsonObject{
                            {KEY_LOGIN, "vadim"},
                            {KEY_PASSWORD, "password"}
                         }).bool_values, BoolValues::True);

    // find user with incorrect login and password
    QCOMPARE(sql.findUser(QJsonObject{
                            {KEY_LOGIN, "not_exist_user"},
                            {KEY_PASSWORD, "not_exist_password"}
                         }).bool_values, BoolValues::Incorrect);

}

void BackendTest::test_generateText()z
{
    QCOMPARE(sql.generateText(Language::ru, 10).value(KEY_TEXT).toString().split(" ").size(), 10);
    QCOMPARE(sql.generateText(Language::en, 10).value(KEY_TEXT).toString().split(" ").size(), 10);
    QCOMPARE(sql.generateText(Language::ru, 20).value(KEY_TEXT).toString().split(" ").size(), 20);
    QCOMPARE(sql.generateText(Language::en, 20).value(KEY_TEXT).toString().split(" ").size(), 20);
    QCOMPARE(sql.generateText(Language::ru, 30).value(KEY_TEXT).toString().split(" ").size(), 30);
    QCOMPARE(sql.generateText(Language::en, 30).value(KEY_TEXT).toString().split(" ").size(), 30);
    QCOMPARE(sql.generateText(Language::ru, 40).value(KEY_TEXT).toString().split(" ").size(), 40);
    QCOMPARE(sql.generateText(Language::en, 40).value(KEY_TEXT).toString().split(" ").size(), 40);
    QCOMPARE(sql.generateText(Language::ru, 50).value(KEY_TEXT).toString().split(" ").size(), 50);
    QCOMPARE(sql.generateText(Language::en, 50).value(KEY_TEXT).toString().split(" ").size(), 50);
    QCOMPARE(sql.generateText(Language::ru, 60).value(KEY_TEXT).toString().split(" ").size(), 1);
    QCOMPARE(sql.generateText(Language::en, 60).value(KEY_TEXT).toString().split(" ").size(), 1);
    QCOMPARE(sql.generateText(Language::ru, -5).value(KEY_TEXT).toString().split(" ").size(), 1);
    QCOMPARE(sql.generateText(Language::en, -5).value(KEY_TEXT).toString().split(" ").size(), 1);
}

void BackendTest::test_generateWords()
{
    QCOMPARE(sql.generateWords(Language::ru, 10).value(KEY_WORDS).toString().split(" ").size(), 10);
    QCOMPARE(sql.generateWords(Language::en, 10).value(KEY_WORDS).toString().split(" ").size(), 10);
    QCOMPARE(sql.generateWords(Language::ru, 20).value(KEY_WORDS).toString().split(" ").size(), 20);
    QCOMPARE(sql.generateWords(Language::en, 20).value(KEY_WORDS).toString().split(" ").size(), 20);
    QCOMPARE(sql.generateWords(Language::ru, 30).value(KEY_WORDS).toString().split(" ").size(), 30);
    QCOMPARE(sql.generateWords(Language::en, 30).value(KEY_WORDS).toString().split(" ").size(), 30);
    QCOMPARE(sql.generateWords(Language::ru, 40).value(KEY_WORDS).toString().split(" ").size(), 40);
    QCOMPARE(sql.generateWords(Language::en, 40).value(KEY_WORDS).toString().split(" ").size(), 40);
    QCOMPARE(sql.generateWords(Language::ru, 50).value(KEY_WORDS).toString().split(" ").size(), 50);
    QCOMPARE(sql.generateWords(Language::en, 50).value(KEY_WORDS).toString().split(" ").size(), 50);
    QCOMPARE(sql.generateWords(Language::ru, 60).value(KEY_WORDS).toString().split(" ").size(), 1);
    QCOMPARE(sql.generateWords(Language::en, 60).value(KEY_WORDS).toString().split(" ").size(), 1);
    QCOMPARE(sql.generateWords(Language::ru, -5).value(KEY_WORDS).toString().split(" ").size(), 1);
    QCOMPARE(sql.generateWords(Language::en, -5).value(KEY_WORDS).toString().split(" ").size(), 1);
}

void BackendTest::test_getProfileStat() // 25 / 25
{
    // get profile of exist user
    QCOMPARE(sql.getProfileStat(123).value(KEY_LOGIN).toString(), "vadim");
    QCOMPARE(sql.getProfileStat(123).value(KEY_DATE_REGISTRATION).toString(), "2023-12-11");
    QCOMPARE(sql.getProfileStat(123).value(KEY_AVERAGE_SPEED).toString(), "100");
    QCOMPARE(sql.getProfileStat(123).value(KEY_MAX_SPEED).toString(), "100");

    // get profile of not exist user
    QCOMPARE(sql.getProfileStat(0).value(KEY_LOGIN).toString(), "not_exist_user");
    QCOMPARE(sql.getProfileStat(0).value(KEY_DATE_REGISTRATION).toString(), "2023-12-11");
    QCOMPARE(sql.getProfileStat(0).value(KEY_AVERAGE_SPEED).toString(), "100");
    QCOMPARE(sql.getProfileStat(0).value(KEY_MAX_SPEED).toString(), "100");
}

void BackendTest::test_changeUsername() // 26 / 33
{
    // change login of exist user
    QCOMPARE(sql.changeUsername(QJsonObject{
        {KEY_USER_ID, "123"},
        {KEY_LOGIN, "vadim_change_login"},
        {KEY_PASSWORD, "password"}
    }), BoolValues::True);
}

void BackendTest::test_changePassword() // 18 / 25
{
    // change password of exist user
    QCOMPARE(sql.changePassword(QJsonObject{
                 {KEY_USER_ID, "123"},
                 {KEY_OLD_PASSWORD, "password"},
                 {KEY_NEW_PASSWORD, "new_password"}
             }), BoolValues::True);

    // change password of exist user with incorrect password
    QCOMPARE(sql.changePassword(QJsonObject{
                 {KEY_USER_ID, "12345"},
                 {KEY_OLD_PASSWORD, "wrong_password"},
                 {KEY_NEW_PASSWORD, "new_password"}
             }), BoolValues::Incorrect);
}

void BackendTest::test_deleteAccount() // 20 / 22
{
    // delete account of exist user
    QCOMPARE(sql.deleteAccount(QJsonObject{
                 {KEY_USER_ID, "123"},
                 {KEY_PASSWORD, "password"}
             }), BoolValues::True);

    // delete account of not exist user_id
    QCOMPARE(sql.deleteAccount(QJsonObject{
                 {KEY_USER_ID, "not_exist_id"},
                 {KEY_PASSWORD, "wrong_password"}
             }), BoolValues::Incorrect);
}

void BackendTest::test_addStatistics() // 47 / 47
{
    // add stat of exist user
    QVERIFY(sql.addStatistic(QJsonObject{
        {KEY_USER_ID, "123"},
        {KEY_SPEED, "100"},
        {KEY_COUNT_MISTAKES, "10"}
    }));

    // add stat of not exist user
    QVERIFY(sql.addStatistic(QJsonObject{
        {KEY_USER_ID, "not_exist_id"},
        {KEY_SPEED, "100"},
        {KEY_COUNT_MISTAKES, "10"}
    }));
}











