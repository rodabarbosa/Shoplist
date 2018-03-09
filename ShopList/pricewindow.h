#ifndef PRICEWINDOW_H
#define PRICEWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QDateTime>
#include "commondata.h"
#include "priceentrywindow.h"

namespace Ui {
    class PriceWindow;
    }

class PriceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PriceWindow(CommonData &commonData, int productID, QWidget *parent = 0);
    ~PriceWindow();

private slots:
    void on_btnExit_clicked();

    void on_btnNew_clicked();

    void on_btnUpdate_clicked();

    void on_btnDelete_clicked();

private:
    Ui::PriceWindow *ui;
    QSqlQueryModel * _model;
    QSqlQuery * _query;
    CommonData * _commonData;
    int _productId;
    int _supermarketId;
    QDate _date;


    void initialize();
    void loadTable();
    void loadProductName();
    void configureTable();
    int getSelectId();
    bool deleteEntry(const int id);
};

#endif // PRICEWINDOW_H
