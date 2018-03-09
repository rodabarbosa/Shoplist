#ifndef PRICEENTRYWINDOW_H
#define PRICEENTRYWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QDateTime>
#include "commondata.h"
#include "brandentrywindow.h"
#include "supermarketentrywindow.h"

namespace Ui {
    class PriceEntryWindow;
    }

class PriceEntryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PriceEntryWindow(CommonData &commonData, const int productId, QWidget *parent = 0);
    ~PriceEntryWindow();

    void setPriceId(const int id);

private slots:
    void on_buttonBox_accepted();

    void on_btnBrand_clicked();

    void on_btnSupermarket_clicked();

private:
    Ui::PriceEntryWindow *ui;
    QSqlQuery * _query;
    QSqlQueryModel * _brandModel;
    QSqlQueryModel * _supermarketModel;
    CommonData *_commonData;
    int _id;
    int _productId;

    void initialize();
    void loadProductName();
    void loadPriceData();
    void loadBrandCombo();
    void loadSupermarketCombo();
    void selectInformedBrand(const int id);
    void selectInformedSupermarket(const int id);
    bool insertEntry();
    bool updateEntry();

    int getSelectedBrandId();
    int getSelectedSupermarketId();


};

#endif // PRICEENTRYWINDOW_H
