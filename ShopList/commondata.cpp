#include "commondata.h"

CommonData::CommonData()
{
    this->_lastSelectedDate = QDate::currentDate();
    this->_lastSelectedSupermarketId = 0;

    QString dirName = QCoreApplication::applicationDirPath() + "/database";
    if (!QDir(dirName).exists()) {
        QDir().mkdir(dirName);
    }

    this->_conn = new DBConnection(dirName + "/shoplist.db");
    //this->_conn = new DBConnection("/home/rod/Dropbox/Projetos/QT/ShopList/Database/shoplist.db");
}

CommonData::~CommonData()
{
    delete this->_conn;
}

DBConnection CommonData::getDBConnection()
{
    return *this->_conn;
}

QDate CommonData::getLastSelectedDate()
{
    return this->_lastSelectedDate;
}

void CommonData::setSelectedDate(QDate date)
{
    this->_lastSelectedDate = date;
}

int CommonData::getLastSelectedSupermarketId()
{
    return this->_lastSelectedSupermarketId;
}

void CommonData::setSelectedSupermarketId(int id)
{
    this->_lastSelectedSupermarketId = id;
}
