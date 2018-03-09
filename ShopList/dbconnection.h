#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

class DBConnection
{
    private:
        QSqlDatabase db;
        int lastInsertedId;

    public:
        DBConnection(const QString &filePath);
        QSqlDatabase getConnection();

        bool open();
        void close();

        int getLastInsertedId();
        void setLastInsertedId(const int id);


};

#endif // DBCONNECTION_H
