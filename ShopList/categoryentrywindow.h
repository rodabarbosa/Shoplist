#ifndef CATEGORYENTRYWINDOW_H
#define CATEGORYENTRYWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include "commondata.h"

namespace Ui {
    class CategoryEntryWindow;
    }

class CategoryEntryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryEntryWindow(CommonData &commonData, const int id, QWidget *parent = 0);
    ~CategoryEntryWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CategoryEntryWindow *ui;
    QSqlQuery * _query;
    CommonData * _commonData;
    int _id;

    void initialize();
    bool insertEntry();
    bool updateEntry();
    void loadFrame();
};

#endif // CATEGORYENTRYWINDOW_H
