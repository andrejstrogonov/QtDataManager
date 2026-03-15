#include "dbmanager.h"
#include <QSqlError>
#include <QDebug>

DbManager::DbManager()
{
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
        m_db.close();
}

DbManager &DbManager::instance()
{
    static DbManager inst;
    return inst;
}

bool DbManager::openDatabase(const QString &host,
                              int port,
                              const QString &dbName,
                              const QString &user,
                              const QString &password,
                              const QString &driver)
{
    if (m_db.isOpen())
        return true;

    m_db = QSqlDatabase::addDatabase(driver);
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);

    if (!m_db.open()) {
        qDebug() << "Database error:" << m_db.lastError().text();
        return false;
    }
    return true;
}

QSqlDatabase DbManager::database() const
{
    return m_db;
}
