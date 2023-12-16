#ifndef BACKENDTEST_H
#define BACKENDTEST_H

#include <QObject>
#include <QJsonObject>
#include <QTest>

#include "server.h"

class BackendTest : public QObject
{
    Q_OBJECT
public:
    explicit BackendTest(QObject *parent = nullptr);

    Sql sql;

private slots:
    void test_getTypeText();

    void test_getLanguage();

    void test_addUser();

    void test_findUser();

    void test_addStatistics();

    void test_getProfileStat();

    void test_changeUsername();

    void test_changePassword();

    void test_deleteAccount();

    void test_generateText();

    void test_generateWords();

    void cleanUp();

signals:

};

#endif // BACKENDTEST_H
