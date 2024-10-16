#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>

#define DB_FILE "db_1.sqlite"
#define DB_TABLE_NAME "person"
#define DB_ID_COLUMN "id"
#define DB_NAME_COLUMN "name"
#define DB_PHONE_COLUMN "phone"
#define DB_EMAIL_COLUMN "email"
#define DB_ROLE_COLUMN "role"

class DBManager : public QObject
{
    Q_OBJECT

public:
    DBManager(QObject *p = nullptr);
    ~DBManager();
    void connectDatabase();
    bool insertTestData();
    QSqlDatabase getDb();
    QString getPrintableRecord(const int id);
    QString getTableName() {
        return DB_TABLE_NAME;
    }
    void closeDb();
private:
    QSqlDatabase db;
    bool openDb();
    bool createDb();
    bool createTable();


public slots:
    int insert(const QVariantList* data);
    bool remove(const int id);
};

#endif // DBMANAGER_H
