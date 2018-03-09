#include "pricewindow.h"
#include "ui_pricewindow.h"

PriceWindow::PriceWindow(CommonData &commonData, int productID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PriceWindow)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_DeleteOnClose);
    this->_commonData = &commonData;
    this->_productId = productID;
    this->initialize();
    this->loadProductName();
    this->loadTable();
}

PriceWindow::~PriceWindow()
{
    delete this->_query;
    delete this->_model;

    delete ui;
}

void PriceWindow::initialize()
{
    this->setWindowFlags(Qt::Window);
    this->_date = QDate::currentDate();
    QWidget::setLayout(ui->gridLayout);
    ui->gridLayout->setAlignment(Qt::AlignTop);
    ui->gridLayout->setMargin(15);
    ui->horizontalLayout->setAlignment(Qt::AlignTop);
    ui->verticalLayout->setAlignment(Qt::AlignTop);

    this->_model = new QSqlQueryModel(ui->tableView);
    this->_query = new QSqlQuery(this->_commonData->getDBConnection().getConnection());
}

void PriceWindow::loadProductName()
{
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT name FROM product WHERE id = :id");
    this->_query->bindValue(":id", this->_productId);
    if (this->_query->exec()) {
        if (this->_query->next()) {
            ui->edtProduct->setText(this->_query->value(0).toString());
        }
    }
    this->_commonData->getDBConnection().close();
}

void PriceWindow::loadTable()
{
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT pc.id, 'R$ ' || pc.value AS value, strftime('%d/%m/%Y',pc.date) AS date, pc.description FROM price pc INNER JOIN brand b ON pc.brand_id = b.id INNER JOIN supermarket s ON pc.supermarket_id = s.id WHERE pc.product_id = :productId ORDER BY pc.date");
    this->_query->bindValue(":productId", this->_productId);
    this->_query->exec();
    this->_model->setQuery(*this->_query);
    ui->tableView->setModel(this->_model);
    this->_commonData->getDBConnection().close();

    this->configureTable();
    ui->lblTotal->setText("Total de registro " + QString::number(this->_model->rowCount()));
}

void PriceWindow::configureTable()
{
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 100);

    ui->tableView->model()->setHeaderData(0, Qt::Horizontal, tr("Código"));
    ui->tableView->model()->setHeaderData(1, Qt::Horizontal, tr("Preço"));
    ui->tableView->model()->setHeaderData(2, Qt::Horizontal, tr("Data"));
    ui->tableView->model()->setHeaderData(3, Qt::Horizontal, tr("Descrição"));
}

int PriceWindow::getSelectId()
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

void PriceWindow::on_btnExit_clicked()
{
    close();
}

void PriceWindow::on_btnNew_clicked()
{
    PriceEntryWindow *w = new PriceEntryWindow(*this->_commonData, this->_productId, this);
    w->exec();
    if (w->result()) {
        this->loadTable();
    }
    delete w;
}

void PriceWindow::on_btnUpdate_clicked()
{
    if (ui->tableView->selectionModel()->selectedRows().count() > 0) {
        PriceEntryWindow *w = new PriceEntryWindow(*this->_commonData, this->_productId, this);
        w->setPriceId(this->getSelectId());
        w->exec();
        if (w->result()) {
            this->loadTable();
        }
        delete w;
    } else {
        QMessageBox::warning(this, "Exclusão", "Nenhum registro selecionado.");
    }
}

void PriceWindow::on_btnDelete_clicked()
{
    if (ui->tableView->selectionModel()->selectedRows().count() > 0) {
        if (QMessageBox::question(this, "Excluir", "Realmente deseja excluir este registro?") == QMessageBox::Yes) {
            int id = this->getSelectId();
            if (this->deleteEntry(id)) {
                this->loadTable();
                QMessageBox::information(this, "Exclusão", "Registro excluido com sucesso.");
            } else {
                QMessageBox::information(this, "Ops", "Não foi possivel excluir o registro.");
            }
        }
    } else {
        QMessageBox::warning(this, "Exclusão", "Nenhum registro selecionado.");
    }
}

bool PriceWindow::deleteEntry(const int id)
{
    bool success = false;
    try {
        this->_query->clear();
        this->_commonData->getDBConnection().open();
        this->_query->prepare("DELETE FROM price WHERE id = :id");
        this->_query->bindValue(":id", id);
        success = this->_query->exec();        
    } catch (std::exception &e) {
        QString message = "Não foi possivel excluir o registro.\n";
        message += e.what();
        QMessageBox::critical(this, "Erro", message);
    }
    this->_commonData->getDBConnection().close();
    return success;
}
