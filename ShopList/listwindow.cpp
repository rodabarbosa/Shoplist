#include "listwindow.h"
#include "ui_listwindow.h"

#include <QMessageBox>

ListWindow::ListWindow(CommonData &commonData, TableType type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListWindow)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_DeleteOnClose);
    this->_commonData = &commonData;
    this->_type = type;
    this->initialize();
    this->loadTable();
}

ListWindow::~ListWindow()
{
    delete this->_query;
    delete this->_proxy;
    delete this->_model;

    delete ui;
}

void ListWindow::loadTable()
{
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare(this->getQuery());
    this->_query->exec();
    this->_model->setQuery(*this->_query);
    this->_proxy->setSourceModel(this->_model);
    ui->tableView->setModel(this->_proxy);
    this->_commonData->getDBConnection().close();

    this->configureTable();
    ui->lblTotal->setText("Total de registro " + QString::number(this->_model->rowCount()));
}

void ListWindow::configureTable() {
    ui->tableView->setColumnWidth(0, 80);
    ui->tableView->setColumnWidth(1, 250);

    ui->tableView->model()->setHeaderData(0, Qt::Horizontal, tr("Código"));
    switch (this->_type) {
        case ttBrand:
            ui->tableView->model()->setHeaderData(1, Qt::Horizontal, tr("Marca"));
        break;

        case ttCategory:
            ui->tableView->model()->setHeaderData(1, Qt::Horizontal, tr("Categoria"));
        break;

        case ttProduct:
            ui->tableView->model()->setHeaderData(1, Qt::Horizontal, tr("Produto"));
            ui->tableView->model()->setHeaderData(2, Qt::Horizontal, tr("Categoria"));
            ui->tableView->model()->setHeaderData(3, Qt::Horizontal, tr("Quantidade"));
            ui->tableView->model()->setHeaderData(4, Qt::Horizontal, tr("Quantidade\nIdeal"));
            ui->tableView->model()->setHeaderData(5, Qt::Horizontal, tr("Quantidade\nMinima"));
        break;

        case ttSupermarket:
            ui->tableView->model()->setHeaderData(1, Qt::Horizontal, tr("Mercado"));
            ui->tableView->model()->setHeaderData(2, Qt::Horizontal, tr("Ativo"));

        break;
    }
}

QString ListWindow::getQuery() {
    switch (this->_type) {
        case ttBrand:
        return "SELECT * FROM brand ORDER BY name";
        break;

        case ttCategory:
        return "SELECT * FROM category ORDER BY name";
        break;

        case ttProduct:
        return "SELECT p.id, p.name, c.name as category, p.quantity, p.ideal_quantity, p.minimum_quantity FROM product p LEFT JOIN category c ON p.category_id = c.id ORDER BY p.name";
        break;

        case ttSupermarket:
        return "SELECT id, name, CASE WHEN active = 0 THEN 'Sim' ELSE 'Não' END AS active FROM supermarket ORDER BY name";
        break;
    }
}

QString ListWindow::getDeleteQuery()
{
    switch (this->_type) {
        case ttBrand:
        return "DELETE FROM brand WHERE id = :id";
        break;

        case ttCategory:
        return "DELETE FROM category WHERE id = :id";
        break;

        case ttProduct:
        return "DELETE FROM product WHERE id = :id";
        break;

        case ttSupermarket:
        return "DELETE FROM supermarket WHERE id = :id";
        break;
    }
}

void ListWindow::initialize()
{
    this->setWindowFlags(Qt::Window);
    QWidget::setLayout(ui->gridLayout);
    ui->gridLayout->setAlignment(Qt::AlignTop);
    ui->gridLayout->setMargin(15);
    ui->horizontalLayout->setAlignment(Qt::AlignTop);
    ui->verticalLayout->setAlignment(Qt::AlignTop);

    this->_model = new QSqlQueryModel(ui->tableView);
    this->_proxy = new QSortFilterProxyModel(this);
    this->_query = new QSqlQuery(this->_commonData->getDBConnection().getConnection());

    this->defineWindowTitle();
    ui->btnPrice->setVisible(this->_type == ttProduct);
}

void ListWindow::defineWindowTitle() {
    switch (this->_type) {
        case ttBrand:
            QWidget::setWindowTitle("Lista de Marcas");
        break;

        case ttCategory:
            QWidget::setWindowTitle("Lista de Categorias");
        break;

        case ttProduct:
            QWidget::setWindowTitle("Lista de Produtos");
        break;

        case ttSupermarket:
            QWidget::setWindowTitle("Lista de Mercados");
        break;
    }
}

void ListWindow::on_btnSearch_clicked()
{
    this->_proxy->setFilterKeyColumn(1);
    this->_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->_proxy->setFilterRegExp(ui->edtSearch->text().trimmed());
}

void ListWindow::on_btnNew_clicked()
{
    QDialog * w;
    switch (this->_type) {
        case ttBrand:
            w = new BrandEntryWindow(*this->_commonData, 0, this);
        break;

        case ttCategory:
            w = new CategoryEntryWindow(*this->_commonData, 0, this);
        break;

        case ttProduct:
            w = new ProductEntryWindow(*this->_commonData, 0, this);
        break;

        case ttSupermarket:
            w = new SupermarketEntryWindow(*this->_commonData, 0, this);
        break;
    }
    w->setModal(true);
    w->show();
    w->exec();
    if (w->result()) {
        this->loadTable();
    }
    delete w;
}

void ListWindow::on_btnUpdate_clicked()
{
    if (ui->tableView->selectionModel()->selectedRows().count() > 0) {
        int id = this->getSelectId();
        QDialog * w;
        switch (this->_type) {
            case ttBrand:
                w = new BrandEntryWindow(*this->_commonData, id, this);
            break;

            case ttCategory:
                w = new CategoryEntryWindow(*this->_commonData, id, this);
            break;

            case ttProduct:
                w = new ProductEntryWindow(*this->_commonData, id, this);
            break;

            case ttSupermarket:
                w = new SupermarketEntryWindow(*this->_commonData, id, this);
            break;
        }
        w->setModal(true);
        w->show();
        w->exec();
        if (w->result()) {
            this->loadTable();
        }
        delete w;
    } else {
        QMessageBox::warning(this, "Seleção", "Nenhum registro selecionado.");
    }
}

void ListWindow::on_btnDelete_clicked()
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

void ListWindow::on_btnExit_clicked()
{
    close();
}

void ListWindow::on_edtSearch_textChanged(const QString &arg1)
{
    this->_proxy->setFilterKeyColumn(1);
    this->_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->_proxy->setFilterRegExp(arg1.trimmed());
}

int ListWindow::getSelectId()
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

bool ListWindow::deleteEntry(const int id)
{
    bool success = false;
    try {
        this->_query->clear();
        this->_commonData->getDBConnection().open();
        this->_query->prepare(this->getDeleteQuery());
        this->_query->bindValue(":id", id);
        success = this->_query->exec();
        this->_commonData->getDBConnection().close();
    } catch (std::exception &e) {
        QString message = "Não foi possivel excluir o registro.\n";
        message += e.what();
        QMessageBox::critical(this, "Erro", message);
    }
    return success;
}

void ListWindow::on_btnPrice_clicked()
{
    if (ui->tableView->selectionModel()->selectedRows().count() > 0) {
        PriceWindow * w = new PriceWindow(*this->_commonData, this->getSelectId(), this);
        w->setModal(true);
        w->show();
        w->exec();
    } else {
        QMessageBox::warning(this, "Exclusão", "Nenhum registro selecionado.");
    }
}
