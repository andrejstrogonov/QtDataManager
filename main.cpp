#include <QApplication>
#include "views/mainwindow.h"
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize database connection
    // Use environment variables when available (useful for Docker), otherwise fall back to defaults.
    const QString dbHost = qEnvironmentVariable("DB_HOST", "localhost");
    const int dbPort = qEnvironmentVariable("DB_PORT").toInt() ? qEnvironmentVariable("DB_PORT").toInt() : 5432;
    const QString dbName = qEnvironmentVariable("DB_NAME", "mydb");
    const QString dbUser = qEnvironmentVariable("DB_USER", "andrei");
    const QString dbPassword = qEnvironmentVariable("DB_PASSWORD", "C0dRVJno");

    if (!DbManager::instance().openDatabase(dbHost, dbPort, dbName, dbUser, dbPassword)) {
        qFatal("Unable to open database");
        return -1;
    }

    MainWindow w;
    w.show();
    return app.exec();
}
