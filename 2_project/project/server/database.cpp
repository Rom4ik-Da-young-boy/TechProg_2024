#include "database.h"
#include <QCoreApplication>
#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QtSql>

Database::Database() {}
void Database::initializeDatabase() {
    initDB();
}
void Database::initDB() {
    this->dbInstance = QSqlDatabase::addDatabase("QSQLITE");
    this->dbInstance.setDatabaseName("database.sqlite");

    // Проверяем, существует ли файл базы данных
    if (!QFile::exists("database.sqlite")) {
        // Если файл не существует, создаем базу данных и таблицы
        if (!this->dbInstance.open()) {
            qDebug() << this->dbInstance.lastError().text();
        }
        this->doSQLQuery("CREATE TABLE IF NOT EXISTS Users (userID INTEGER PRIMARY KEY, login VARCHAR (255) UNIQUE NOT NULL, password VARCHAR (255) NOT NULL, token VARCHAR (255) UNIQUE NOT NULL)");
    } else {
        // Если файл существует, просто открываем базу данных
        if (!this->dbInstance.open()) {
            qDebug() << this->dbInstance.lastError().text();
        }
    }
}

Database::~Database() {
    this->dbInstance.close();
}

QSqlQuery Database::doSQLQuery(QString stringQuery) {
    if (!dbInstance.isValid() || !dbInstance.isOpen()) {
        initDB(); // Инициализируем базу данных, если она еще не инициализирована
    }

    QSqlQuery query(dbInstance);
    bool status = query.exec(stringQuery);
    qDebug().noquote() << QString("Query %1, status %2").arg(stringQuery).arg(status);
    if (!status) {
        dbInstance.rollback();
    } else {
        dbInstance.commit();
    }
    return query;
}

Database* Database::pInstance = nullptr;
QSqlDatabase Database::dbInstance;
DatabaseDestroyer Database::destroyer;
