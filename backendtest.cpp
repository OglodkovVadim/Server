#include "backendtest.h"
#include "qtestcase.h"

BackendTest::BackendTest(QObject *parent)
    : QObject{parent}
{ 
}

void BackendTest::test_getTypeText()
{
    Server server;
    QCOMPARE(server.getTextType("text"), TextType::text);
    QCOMPARE(server.getTextType("words"), TextType::words);
}

void BackendTest::test_getLanguage()
{
    Server server;
    QCOMPARE(server.getLanguage("ru"), Language::ru);
    QCOMPARE(server.getLanguage("en"), Language::en);
}

void BackendTest::test_addUser()
{
    Sql sql;
    QCOMPARE(sql.addUser(QJsonObject{
                {KEY_LOGIN, "vadim"},
                {KEY_PASSWORD, "password"}
             }).bool_values, BoolValues::True);

    QCOMPARE(sql.addUser(QJsonObject{
                {KEY_LOGIN, "user"},
                {KEY_PASSWORD, "123"}
             }).bool_values, BoolValues::Incorrect);
}

void BackendTest::test_findUser()
{
    Sql sql;
    QCOMPARE(sql.findUser(QJsonObject{
                            {KEY_LOGIN, "vadim"},
                            {KEY_PASSWORD, "password"}
                         }).bool_values, BoolValues::True);

    QCOMPARE(sql.findUser(QJsonObject{
                            {KEY_LOGIN, "not_exist_user"},
                            {KEY_PASSWORD, "not_exist_password"}
                         }).bool_values, BoolValues::Incorrect);

    QCOMPARE(sql.findUser(QJsonObject{
                              {KEY_LOGIN, "not_exist_user"},
                              {KEY_PASSWORD, "not_exist_password"}
                          }).bool_values, BoolValues::True);
}

void BackendTest::test_getProfileStat()
{

}

void BackendTest::test_addStatistics()
{
    Sql sql;
    QVERIFY(sql.addStatistic(QJsonObject{
        {KEY_USER_ID, "394476"},
        {KEY_SPEED, "100"},
        {KEY_COUNT_MISTAKES, "10"}
    }));

    QVERIFY(sql.addStatistic(QJsonObject{
        {KEY_USER_ID, "-1"},
        {KEY_SPEED, "100"},
        {KEY_COUNT_MISTAKES, "10"}
    }));

}
