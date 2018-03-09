#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QDialog>
#include <QString>
#include <QSortFilterProxyModel>
#include "commondata.h"

#include "brandentrywindow.h"
#include "categoryentrywindow.h"
#include "productentrywindow.h"
#include "pricewindow.h"
#include "supermarketentrywindow.h"

namespace Ui {
    class ListWindow;
}

enum TableType { ttBrand, ttCategory, ttProduct, ttSupermarket };

class ListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ListWindow(CommonData &commonData, TableType type, QWidget *parent = 0);
    ~ListWindow();

private slots:
    void on_btnSearch_clicked();

    void on_btnNew_clicked();

    void on_btnUpdate_clicked();

    void on_btnDelete_clicked();

    void on_btnExit_clicked();

    void on_edtSearch_textChanged(const QString &arg1);

    void on_btnPrice_clicked();

private:
    Ui::ListWindow *ui;
    QSqlQueryModel * _model;
    QSqlQuery * _query;
    QSortFilterProxyModel * _proxy;
    TableType _type;
    CommonData * _commonData;

    void initialize();
    void loadTable();
    void configureTable();
    QString getQuery();
    QString getDeleteQuery();
    void defineWindowTitle();
    int getSelectId();
    bool deleteEntry(const int id);
};

#endif // LISTWINDOW_H
