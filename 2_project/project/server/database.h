#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSql>
#include <QFile>

class Database;

class DatabaseDestroyer {
private:
    Database *pInstance;
public:
    ~DatabaseDestroyer() {
        delete this->pInstance;
    }
    void initialize(Database *p) {
        this->pInstance = p;
    }
};

class Database {
private:
    static Database *pInstance;

    void initDB();
    static QSqlDatabase dbInstance;
    static DatabaseDestroyer destroyer;

protected:
    Database();
    Database(const Database&) = delete;
    Database& operator=(Database&) = delete;

    ~Database();
    friend class DatabaseDestroyer;

public:
    static Database* getInstance() {
        if (!pInstance) {
            pInstance = new Database();
            destroyer.initialize(pInstance);
        }
        return pInstance;
    }

    void initializeDatabase(); // Объявление нового метода для инициализации базы данных

    QSqlQuery doSQLQuery(QString stringQuery);
};

#endif // DATABASE_H
