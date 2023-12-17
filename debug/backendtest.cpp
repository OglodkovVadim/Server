#include "backendtest.h"
#include "qtestcase.h"

BackendTest::BackendTest(QObject *parent)
    : QObject{parent}
{
    sql.init();
}

void BackendTest::test_getTypeText() // 1 / 1
{
    // Arrange
    Server server;
    server.init();

    // Act
    TextType type_text = server.getTextType("text");
    TextType type_words = server.getTextType("words");

    // Assert
    QCOMPARE(type_text, TextType::text);
    QCOMPARE(type_words, TextType::words);

}

void BackendTest::test_getLanguage() // 1 / 1
{
    // Arrange
    Server server;
    server.init();

    // Act
    Language language_ru = server.getLanguage("ru");
    Language language_en = server.getLanguage("en");

    // Assert
    QCOMPARE(language_ru, Language::ru);
    QCOMPARE(language_en, Language::en);
}

void BackendTest::test_addUser() // 26 / 34
{
    // Arrange
    QJsonObject not_exist_user;
    not_exist_user.insert(KEY_LOGIN, "Not_exist_login");
    not_exist_user.insert(KEY_PASSWORD, "Not_exist_password");

    QJsonObject exist_user;
    exist_user.insert(KEY_LOGIN, "Login");
    exist_user.insert(KEY_PASSWORD, "Password");

    // Act
    Auth auth_not_exist = sql.addUser(not_exist_user);
    Auth auth_exist = sql.addUser(exist_user);

    // Assert
    QCOMPARE(auth_not_exist.bool_values, BoolValues::True);
    QCOMPARE(auth_exist.bool_values, BoolValues::Incorrect);

    sql.clenUpDB();
}

void BackendTest::test_findUser() // 15 / 21
{
    // Arrange
    QJsonObject not_exist_user;
    not_exist_user.insert(KEY_LOGIN, "Not_exist_Login");
    not_exist_user.insert(KEY_PASSWORD, "Not_exist_Password");

    QJsonObject exist_user;
    exist_user.insert(KEY_LOGIN, "Login");
    exist_user.insert(KEY_PASSWORD, "Password");

    // Act
    Auth auth_not_exist = sql.findUser(not_exist_user);
    Auth auth_exist = sql.findUser(exist_user);

    // Assert
    QCOMPARE(auth_exist.bool_values, BoolValues::True);
    QCOMPARE(auth_not_exist.bool_values, BoolValues::Incorrect);
}

void BackendTest::test_generateText()
{
    // Arrange
    struct TestData {
        Language language;
        int wordCount;
    };

    TestData normalData[] = {
        {Language::ru, 10},
        {Language::en, 10},
        {Language::ru, 20},
        {Language::en, 20},
        {Language::ru, 30},
        {Language::en, 30},
        {Language::ru, 40},
        {Language::en, 40},
        {Language::ru, 50},
        {Language::en, 50}
    };

    TestData wrongData[] = {
        {Language::ru, 60},
        {Language::en, 60},
        {Language::ru, -5},
        {Language::en, -5}
    };

    QVector<int> list_size_normal_data;
    QVector<int> list_size_wrong_data;

    // Act
    for (const auto& data : normalData)
        list_size_normal_data.push_back(sql.generateText(data.language, data.wordCount).value(KEY_TEXT).toString().split(" ").size());
    for (const auto& data : wrongData)
        list_size_wrong_data.push_back(sql.generateText(data.language, data.wordCount).value(KEY_TEXT).toString().split(" ").size());

    // Assert
    for (int i = 0; i < list_size_normal_data.size(); i++)
        QCOMPARE(list_size_normal_data[i], normalData[i].wordCount);
    for (const auto& data : list_size_wrong_data)
        QCOMPARE(data, 1);
}

void BackendTest::test_generateWords()
{
    // Arrange
    struct TestData {
        Language language;
        int wordCount;
    };

    TestData normalData[] = {
        {Language::ru, 10},
        {Language::en, 10},
        {Language::ru, 20},
        {Language::en, 20},
        {Language::ru, 30},
        {Language::en, 30},
        {Language::ru, 40},
        {Language::en, 40},
        {Language::ru, 50},
        {Language::en, 50}
    };

    TestData wrongData[] = {
        {Language::ru, 60},
        {Language::en, 60},
        {Language::ru, -5},
        {Language::en, -5}
    };
    QVector<int> list_size_normal_data;
    QVector<int> list_size_wrong_data;

    // Act
    for (const auto& data : normalData)
        list_size_normal_data.push_back(sql.generateWords(data.language, data.wordCount).value(KEY_WORDS).toString().split(" ").size());
    for (const auto& data : wrongData)
        list_size_wrong_data.push_back(sql.generateWords(data.language, data.wordCount).value(KEY_WORDS).toString().split(" ").size());

    // Assert
    for (int i = 0; i < list_size_normal_data.size(); i++)
        QCOMPARE(list_size_normal_data[i], normalData[i].wordCount);
    for (const auto& data : list_size_wrong_data)
        QCOMPARE(data, 1);
}

void BackendTest::test_getProfileStat() // 25 / 25
{
    // Arrange
    uint32_t exist_id = 123;
    uint32_t not_exist_id = 0;

    // Act
    QJsonObject exist_user = sql.getProfileStat(exist_id);
    QJsonObject not_exist_user = sql.getProfileStat(not_exist_id);

    // Assert
    QCOMPARE(exist_user.isEmpty(), false);
    QCOMPARE(not_exist_user.isEmpty(), true);
}

void BackendTest::test_changeUsername() // 26 / 33
{
    // Arrange
    QJsonObject exist_user;
    QJsonObject not_exist_user;

    exist_user.insert(KEY_USER_ID, "123");
    exist_user.insert(KEY_LOGIN, "New_Login");
    exist_user.insert(KEY_PASSWORD, "Password");

    not_exist_user.insert(KEY_USER_ID, "0");
    not_exist_user.insert(KEY_LOGIN, "Not_exist_login");
    not_exist_user.insert(KEY_PASSWORD, "Not_exist_password");

    // Act
    BoolValues exist = sql.changeUsername(exist_user);
    BoolValues not_exist = sql.changeUsername(not_exist_user);

    // Assert
    QCOMPARE(exist, BoolValues::True);
    QCOMPARE(not_exist, BoolValues::Incorrect);

    sql.clenUpDB();
}

void BackendTest::test_changePassword() // 18 / 25
{
    // Arrange
    QJsonObject exist_user;
    QJsonObject not_exist_user;

    exist_user.insert(KEY_USER_ID, "123");
    exist_user.insert(KEY_OLD_PASSWORD, "Password");
    exist_user.insert(KEY_NEW_PASSWORD, "New_password");

    not_exist_user.insert(KEY_USER_ID, "0");
    not_exist_user.insert(KEY_OLD_PASSWORD, "Not_exist_password");
    not_exist_user.insert(KEY_NEW_PASSWORD, "Not_exist_password");

    // Act
    BoolValues exist = sql.changePassword(exist_user);
    BoolValues not_exist = sql.changePassword(not_exist_user);

    // Assert
    QCOMPARE(exist, BoolValues::True);
    QCOMPARE(not_exist, BoolValues::Incorrect);

    sql.clenUpDB();
}

void BackendTest::test_deleteAccount() // 20 / 22
{
    // Arrange
    QJsonObject exist_user;
    QJsonObject not_exist_user;

    exist_user.insert(KEY_USER_ID, "123");
    exist_user.insert(KEY_PASSWORD, "Password");

    not_exist_user.insert(KEY_USER_ID, "0");
    not_exist_user.insert(KEY_PASSWORD, "Not_exist_password");

    // Act
    BoolValues exist = sql.deleteAccount(exist_user);
    BoolValues not_exist = sql.deleteAccount(not_exist_user);

    // Assert
    QCOMPARE(exist, BoolValues::True);
    QCOMPARE(not_exist, BoolValues::Incorrect);

    sql.clenUpDB();
}

void BackendTest::test_addStatistics() // 47 / 47
{
    // Arrange
    QJsonObject exist_user;
    QJsonObject not_exist_user;

    exist_user.insert(KEY_USER_ID, "123");
    exist_user.insert(KEY_SPEED, "100");
    exist_user.insert(KEY_COUNT_MISTAKES, "100");

    not_exist_user.insert(KEY_USER_ID, "0");
    not_exist_user.insert(KEY_SPEED, "0");
    exist_user.insert(KEY_COUNT_MISTAKES, "0");

    // Act
    bool exist = sql.addStatistic(exist_user);
    bool not_exist = sql.addStatistic(not_exist_user);

    // Assert
    QCOMPARE(exist, true);
    QCOMPARE(not_exist, false);

    sql.clenUpDB();
}

void BackendTest::cleanupTestCase()
{
    sql.clenUpDB();

}








