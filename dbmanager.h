#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>

class DbManager
{
public:
    static DbManager &instance();

    bool openDatabase(const QString &host,
                      int port,
                      const QString &dbName,
                      const QString &user,
                      const QString &password,
                      const QString &driver = "QPSQL");

    QSqlDatabase database() const;

private:
    DbManager();
    ~DbManager();

    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
