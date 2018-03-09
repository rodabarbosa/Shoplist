#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTextDocument>
#include <QPrinter>
#include <QDesktopServices>
#include "commondata.h"
#include "listwindow.h"
#include "pricewindow.h"
#include "productentrywindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_BtnRefresh_clicked();

    void on_actionExit_triggered();

    void on_actionBrand_triggered();

    void on_actionCategory_triggered();

    void on_actionProduct_triggered();

    void on_actionSupermarket_triggered();

    void on_btnPrint_clicked();

    void on_btnProductEdit_clicked();

    void on_btnPrice_clicked();

    int getSelectId();
private:
    Ui::MainWindow *ui;
    CommonData * _commomData;
    QSqlQueryModel * _model;
    QSqlQuery * _query;

    void initialize();
    void loadTable();
    void createTableIfNotExists();
};

#endif // MAINWINDOW_H
