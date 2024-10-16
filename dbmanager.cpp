#include "dbmanager.h"
#include <QFile>
#include <QDebug>
#include <QSqlQuery>

DBManager::DBManager(QObject *p)
{

}

DBManager::~DBManager()
{

}

void DBManager::connectDatabase()
{
    if(!QFile(DB_FILE).exists()){
        qDebug() << "Need to create DB";
        bool result = this->createDb();
        qDebug() << "Result: " << result;
    } else {
        qDebug() << "Need to open DB";
        bool result = this->openDb();
        qDebug() << "Result: " << result;
    }
}

bool DBManager::openDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DB_FILE);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

bool DBManager::createDb()
{
    if(this->openDb()){
        bool result = this->createTable();
        return result;
    } else {
        return false;
    }
}

void DBManager::closeDb()
{
    db.close();
}

bool DBManager::createTable()
{
    QSqlQuery query;
    if(!query.exec("CREATE TABLE " DB_TABLE_NAME " ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   DB_NAME_COLUMN " VARCHAR(255),"
                   DB_PHONE_COLUMN " VARCHAR(255),"
                   DB_EMAIL_COLUMN " VARCHAR(255),"
                   DB_ROLE_COLUMN " VARCHAR(255)"
                   " )")) {
        return false;
    }
    else {
        if(insertTestData()) {
            return true;
        }
    }
    return false;
}

int DBManager::insert(const QVariantList* data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " DB_TABLE_NAME " ( "
                  DB_NAME_COLUMN ", "
                  DB_PHONE_COLUMN ", "
                  DB_EMAIL_COLUMN ", "
                  DB_ROLE_COLUMN ") "
                  "VALUES (:pName, :pPhone, :pEmail, :pRole)");
    query.bindValue(":pName", data->at (0).toString ());
    query.bindValue(":pPhone", data->at (1).toString ());
    query.bindValue(":pEmail", data->at (2).toString ());
    query.bindValue(":pRole", data->at (3).toString ());

    if(!query.exec()){
        qDebug() << "error insert into " << DB_TABLE_NAME;
        return -1;
    } else {
        qint16 id = query.lastInsertId ().toInt ();
        return id;
    }
    return -1;
}

bool DBManager::remove(const int id)
{
    QSqlQuery query;
    query.prepare ("DELETE FROM " DB_TABLE_NAME " WHERE id = :id");
    query.bindValue (":id", id);
    if(!query.exec ()) {
        qDebug() << "error deleting id" << id;
        return false;
    }
    else {
        return true;
    }
    return false;
}

QString DBManager::getPrintableRecord(const int id)
{
    QSqlQuery query;
    query.prepare ("SELECT id, name, email, phone, role FROM " DB_TABLE_NAME " WHERE id = :id");
    query.bindValue (":id", id);
    if(query.exec ()) {
        while (query.next()) {
            int id = query.value (0).toInt ();
            QString name = query.value(1).toString();
            QString email = query.value(2).toString();
            QString phone = query.value(3).toString();
            QString role = query.value(4).toString();
            QString result = QString::number (id) + " " + name + " " + email + " " + phone + " " + role;
            qDebug() << result;
            return result;
        }
    }
    else return "";
}

bool DBManager::insertTestData()
{
    QSqlQuery query;
    QString sql = "INSERT INTO " DB_TABLE_NAME " (" DB_NAME_COLUMN ", " DB_PHONE_COLUMN ", " DB_EMAIL_COLUMN ", " DB_ROLE_COLUMN ")"
                  " VALUES('Иван Петров', '+78889991122', 'zxc@mail.ru', 'USER')";
    qDebug() << sql;
    if(!query.exec(sql)) {
        return false;
    }
    else {
        return true;
    }
}

QSqlDatabase DBManager::getDb()
{
    return db;
}
