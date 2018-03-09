#ifndef COMMOMDATA_H
#define COMMOMDATA_H

#include "dbconnection.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>


class CommonData
{
    private:
        DBConnection *_conn;
        QDate _lastSelectedDate;
        int _lastSelectedSupermarketId;

    public:
        CommonData();
        ~CommonData();

        DBConnection getDBConnection();

        QDate getLastSelectedDate();
        void setSelectedDate(QDate date);

        int getLastSelectedSupermarketId();
        void setSelectedSupermarketId(int id);
};

#endif // COMMOMDATA_H
