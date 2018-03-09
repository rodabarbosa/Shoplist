#include "dbconnection.h"

DBConnection::DBConnection(const QString &filePath)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath);


}

QSqlDatabase DBConnection::getConnection()
{
    return db;
}

bool DBConnection::open()
{
    return db.open();
}

void DBConnection::close()
{
    db.close();
}

int DBConnection::getLastInsertedId()
{
    return this->lastInsertedId;
}

void DBConnection::setLastInsertedId(const int id)
{
    this->lastInsertedId = id;
}
