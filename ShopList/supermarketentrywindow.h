#ifndef SUPERMARKETENTRYWINDOW_H
#define SUPERMARKETENTRYWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include "commondata.h"

namespace Ui {
    class SupermarketEntryWindow;
    }

class SupermarketEntryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SupermarketEntryWindow(CommonData &commonData, const int id, QWidget *parent = 0);
    ~SupermarketEntryWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SupermarketEntryWindow *ui;
    QSqlQuery * _query;
    CommonData * _commonData;

    int _id;

    void initialize();
    bool insertEntry();
    bool updateEntry();
    void loadFrame();
};

#endif // SUPERMARKETENTRYWINDOW_H
