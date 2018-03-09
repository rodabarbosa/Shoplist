#ifndef PRODUCTENTRYWINDOW_H
#define PRODUCTENTRYWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include "commondata.h"

namespace Ui {
    class ProductEntryWindow;
    }

class ProductEntryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProductEntryWindow(CommonData &commonData, const int id, QWidget *parent = 0);
    ~ProductEntryWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ProductEntryWindow *ui;
    QSqlQuery * _query;
    QSqlQuery * _qryCategory;
    QSqlQueryModel * _model;
    CommonData * _commonData;
    int _id;

    void initialize();
    bool insertEntry();
    bool updateEntry();
    void loadFrame();
    void loadCategories();
    int getSelectCategoryId();
};

#endif // PRODUCTENTRYWINDOW_H
