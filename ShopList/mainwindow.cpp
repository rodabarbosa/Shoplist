#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_DeleteOnClose);
    this->initialize();
    this->loadTable();
}

MainWindow::~MainWindow()
{
    delete this->_model;
    delete this->_query;
    delete this->_commomData;
    delete ui;
}

void MainWindow::loadTable()
{
    this->_commomData->getDBConnection().open();
    this->_query->prepare("SELECT p.id, p.name, (p.ideal_quantity - p.quantity) as qtd, strftime('%d/%m/%Y',tmp.date) AS date, 'R$ ' || pc.value AS value, pc.value as price, s.name AS supermarket, pc.description FROM product p LEFT JOIN (SELECT id, product_id, MAX(date) AS date FROM price GROUP BY product_id) tmp ON tmp.product_id = p.id LEFT JOIN price pc ON tmp.id = pc.id LEFT JOIN supermarket s ON s.id = pc.supermarket_id WHERE p.quantity < p.ideal_quantity ORDER BY p.name");
    this->_query->exec();
    this->_model->setQuery(*this->_query);

    this->_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    this->_model->setHeaderData(1, Qt::Horizontal, tr("Produto"));
    this->_model->setHeaderData(2, Qt::Horizontal, tr("Quantidade"));
    this->_model->setHeaderData(3, Qt::Horizontal, tr("Data"));
    this->_model->setHeaderData(4, Qt::Horizontal, tr("Preço"));
    this->_model->setHeaderData(5, Qt::Horizontal, tr("value"));
    this->_model->setHeaderData(6, Qt::Horizontal, tr("Mercado"));

    ui->tableView->setModel(this->_model);
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnHidden(5, true);
    ui->tableView->setColumnHidden(7, true);
    this->_commomData->getDBConnection().close();
}

void MainWindow::on_BtnRefresh_clicked()
{
    this->loadTable();
}

void MainWindow::initialize()
{
    ui->centralWidget->setLayout(ui->horizontalLayout);
    ui->horizontalLayout->setAlignment(Qt::AlignTop);
    ui->verticalLayout->setAlignment(Qt::AlignTop);
    ui->horizontalLayout->setMargin(15);

    this->_commomData = new CommonData();
    this->_model = new QSqlQueryModel(this);
    this->_query = new QSqlQuery(this->_commomData->getDBConnection().getConnection());

}

void MainWindow::createTableIfNotExists()
{
    QString createTables = "CREATE TABLE IF NOT EXISTS brand (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR (120) NOT NULL UNIQUE);";
    createTables += "CREATE TABLE category (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR (120) UNIQUE NOT NULL);";
    createTables += "CREATE TABLE price (id INTEGER PRIMARY KEY AUTOINCREMENT, value DOUBLE NOT NULL DEFAULT (0), date DATE NOT NULL DEFAULT (CURRENT_DATE), product_id INTEGER REFERENCES product (id) NOT NULL, brand_id INTEGER NOT NULL REFERENCES brand (id), supermarket_id INTEGER REFERENCES supermarket (id) NOT NULL, description VARCHAR (255));";
    createTables += "CREATE TABLE product (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR (120) UNIQUE NOT NULL, quantity INTEGER DEFAULT (0) NOT NULL, ideal_quantity INTEGER NOT NULL DEFAULT (0), minimum_quantity INTEGER NOT NULL DEFAULT (0), category_id INTEGER REFERENCES category (id));";
    createTables += "CREATE TABLE IF NOT EXISTS supermarket (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(120) NOT NULL UNIQUE, active BOOLEAN NOT NULL DEFAULT true );";

    this->_commomData->getDBConnection().open();
    this->_query->prepare(createTables);
    this->_commomData->getDBConnection().close();
    this->_query->clear();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionBrand_triggered()
{
   ListWindow * w = new ListWindow(*this->_commomData, ttBrand, this);
   w->showMaximized();
   w->setWindowModality(Qt::WindowModal);
   w->setModal(true);
   w->exec();
   delete w;
}

void MainWindow::on_actionCategory_triggered()
{
    ListWindow * w = new ListWindow(*this->_commomData, ttCategory, this);
    w->showMaximized();
    w->setWindowModality(Qt::WindowModal);
    w->setModal(true);
    w->exec();
    delete w;
}

void MainWindow::on_actionProduct_triggered()
{
    ListWindow * w = new ListWindow(*this->_commomData, ttProduct, this);
    w->showMaximized();
    w->setModal(true);
    w->exec();
    this->loadTable();
    delete w;
}

void MainWindow::on_actionSupermarket_triggered()
{
    ListWindow * w = new ListWindow(*this->_commomData, ttSupermarket, this);
    w->showMaximized();
    w->exec();
    delete w;
}

void MainWindow::on_btnPrint_clicked()
{
    QString fileName = "/home/rod/Documentos/test.pdf";
    QString html = "<style>";
    html += "table { font-size: 10px; border: 1px solid black }";
    html += "th { text-align: left; padding-right: 2em; }";
    html += "</style>";
    html += "<h1><center>Lista de compra - " + QDate::currentDate().toString() + "</center></h1>";
    html += "<table>";
    html += "<thead><tr>";
    html += "<th>Produto</th>";
    html += "<th>Qtd</th>";
    html += "<th>Preço</th>";
    html += "<th>Data</th>";
    html += "<th>Mercado</th>";
    html += "<th>Descrição</th>";
    html += "</tr></thead>";
    html += "<tbody>";
    double total = 0;
    this->_query->first();
    while (this->_query->next()) {
        total += this->_query->value(5).toDouble();
        html += "<tr>";
        html += "<td>" + this->_query->value(1).toString() + "</td>";
        html += "<td>" + this->_query->value(2).toString() + "</td>";
        html += "<td>" + this->_query->value(4).toString() + "</td>";
        html += "<td>" + this->_query->value(3).toString() + "</td>";
        html += "<td>" + this->_query->value(6).toString() + "</td>";
        html += "<td>" + this->_query->value(7).toString() + "</td>";
        html += "</tr>";
    }

    html += "</tbody>";
    html += "</table>";

    QTextDocument doc;
    doc.setHtml(html);

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);
    doc.print(&printer);
    QDesktopServices::openUrl(QUrl("file://"+ fileName));
}

void MainWindow::on_btnProductEdit_clicked()
{
    if (ui->tableView->selectionModel()->selectedRows().count() > 0) {
        int productId = this->getSelectId();
        ProductEntryWindow * w = new ProductEntryWindow(*this->_commomData, productId, this);
        w->setWindowModality(Qt::WindowModal);
        w->setModal(true);
        w->exec();
        if (w->result()) {
            this->loadTable();
        }
        delete w;
    } else {
        QMessageBox::warning(this, "Produto", "Nenhum produto selecionado.");
    }
}

void MainWindow::on_btnPrice_clicked()
{
    if (ui->tableView->selectionModel()->selectedRows().count() > 0) {
        int productId = this->getSelectId();
        PriceWindow * w = new PriceWindow(*this->_commomData, productId, this);
        w->showMaximized();
        w->setWindowModality(Qt::WindowModal);
        w->setModal(true);
        w->exec();
        this->loadTable();

        delete w;
    } else {
        QMessageBox::warning(this, "Produto", "Nenhum produto selecionado.");
    }
}

int MainWindow::getSelectId()
{
    QVariant variant;
    QModelIndex index;
    int rowIdx;

    index = ui->tableView->currentIndex();
    rowIdx = index.row();
    index = index.sibling(rowIdx, 0); /* 0 is the code of id column */

    variant = index.data();
    return variant.toInt();
}
