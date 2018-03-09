#ifndef BRANDENTRYWINDOW_H
#define BRANDENTRYWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include "commondata.h"

namespace Ui {
    class BrandEntryWindow;
    }

class BrandEntryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BrandEntryWindow(CommonData &commonData, const int id, QWidget *parent = 0);
    ~BrandEntryWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::BrandEntryWindow *ui;
    QSqlQuery * _query;
    CommonData * _commonData;

    int _id;
    void initialize();
    bool updateEntry();
    bool insertEntry();
    void loadFrame();
};

#endif // BRANDENTRYWINDOW_H
