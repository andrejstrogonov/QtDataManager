#include <QtTest/QtTest>
#include "dbmanager.h"

class TestDbManager : public QObject
{
    Q_OBJECT

private slots:
    void testInvalidConnection();
    void testSqliteMemory();
};

void TestDbManager::testInvalidConnection()
{
    // attempt to open with wrong parameters
    bool ok = DbManager::instance().openDatabase("invalid", 0, "", "", "", "QPSQL");
    QVERIFY(!ok);
}

void TestDbManager::testSqliteMemory()
{
    // use SQLite - should succeed
    bool ok = DbManager::instance().openDatabase("", 0, ":memory:", "", "", "QSQLITE");
    QVERIFY(ok);
}

QTEST_MAIN(TestDbManager)
#include "test_dbmanager.moc"
