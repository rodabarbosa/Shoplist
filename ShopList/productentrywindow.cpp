#include "productentrywindow.h"
#include "ui_productentrywindow.h"

ProductEntryWindow::ProductEntryWindow(CommonData &commonData, const int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductEntryWindow)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_DeleteOnClose);
    this->_commonData = &commonData;
    this->_id = id;
    this->initialize();

    if (this->_id > 0) {
        this->loadFrame();
    }
}

ProductEntryWindow::~ProductEntryWindow()
{
    delete this->_query;
    delete this->_qryCategory;
    delete this->_model;
    delete ui;
}

void ProductEntryWindow::initialize()
{
    this->setLayout(ui->formLayout);
    ui->formLayout->setMargin(15);

    this->_query = new QSqlQuery(this->_commonData->getDBConnection().getConnection());
    this->_qryCategory = new QSqlQuery(this->_commonData->getDBConnection().getConnection());
    this->_model = new QSqlQueryModel(this);

    this->loadCategories();
}

void ProductEntryWindow::loadCategories()
{
    this->_commonData->getDBConnection().open();
    this->_qryCategory->prepare("SELECT name, id FROM category ORDER BY name");
    this->_qryCategory->exec();
    this->_model->setQuery(*this->_qryCategory);
    ui->cbxCategory->setModel(this->_model);
    this->_commonData->getDBConnection().close();
}

void ProductEntryWindow::loadFrame()
{
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT p.id, p.name, p.quantity, p.ideal_quantity, p.minimum_quantity,c.name as category FROM product p INNER JOIN category c ON p.category_id = c.id WHERE p.id = :id");
    this->_query->bindValue(":id", this->_id);

    if (this->_query->exec()) {
        if (this->_query->next()) {
            ui->edtId->setText(this->_query->value(0).toString());
            ui->edtName->setText(this->_query->value(1).toString());
            ui->spbQuantity->setValue(this->_query->value(2).toInt());
            ui->spbIdealQuantity->setValue(this->_query->value(2).toInt());
            ui->spbMinQuantity->setValue(this->_query->value(3).toInt());

            QString category = this->_query->value(4).toString();
            for (int i = 0; i < ui->cbxCategory->count(); i++) {
                if (ui->cbxCategory->itemText(i) == category) {
                    ui->cbxCategory->setCurrentIndex(i);
                    break;
                }
            }
        }
    }
    this->_commonData->getDBConnection().close();
}

void ProductEntryWindow::on_buttonBox_accepted()
{
    bool success = false;
    try {
        if (this->_id > 0) {
            success = this->updateEntry();
        } else {
            success = this->insertEntry();
        }

        if (success) {
            QMessageBox::information(this, "Sucesso", "Registro salvo com successo.");
        } else {
            QMessageBox::warning(this, "Ops", "Registro não foi salvo.");
        }
    } catch (std::exception &e) {
        QString message = "Erro ao salvar registro.\n";
        message += e.what();
        QMessageBox::warning(this, "Erro", message);
    }
    this->_commonData->getDBConnection().close();
}

bool ProductEntryWindow::insertEntry()
{
    bool success = false;
    this->_commonData->getDBConnection().open();
    this->_query->prepare("INSERT INTO product(name, quantity, ideal_quantity, minimum_quantity, category_id) VALUES(:name, :quantity, :ideal_quantity, :minimum_quantity, :category_id)");
    this->_query->bindValue(":name", ui->edtName->text());
    this->_query->bindValue(":quantity", ui->spbQuantity->text());
    this->_query->bindValue(":ideal_quantity", ui->spbIdealQuantity->text());
    this->_query->bindValue(":minimum_quantity", ui->spbMinQuantity->text());
    this->_query->bindValue(":category_id", this->getSelectCategoryId());
    success = this->_query->exec();
    if (success) {
        this->_commonData->getDBConnection().setLastInsertedId(this->_query->lastInsertId().toInt());
    }
    return success;
}

bool ProductEntryWindow::updateEntry()
{
    bool success = false;
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("UPDATE product SET name = :name, quantity = :quantity, ideal_quantity = :ideal_quantity, minimum_quantity = :minimum_quantity, category_id = :category_id WHERE id = :id");
    this->_query->bindValue(":name", ui->edtName->text());
    this->_query->bindValue(":quantity", ui->spbQuantity->text());
    this->_query->bindValue(":ideal_quantity", ui->spbIdealQuantity->text());
    this->_query->bindValue(":minimum_quantity", ui->spbMinQuantity->text());
    this->_query->bindValue(":category_id", this->getSelectCategoryId());
    this->_query->bindValue(":id", ui->edtId->text());
    success = this->_query->exec();

    return success;
}

int ProductEntryWindow::getSelectCategoryId()
{
    this->_qryCategory->clear();
    this->_commonData->getDBConnection().open();
    this->_qryCategory->prepare("SELECT id FROM category WHERE name = :name");
    this->_qryCategory->bindValue(":name", ui->cbxCategory->currentText());

    int id = 0;
    if (this->_qryCategory->exec()) {
        if (this->_qryCategory->next()) {
            id = this->_qryCategory->value(0).toInt();
        }
    }
    this->_commonData->getDBConnection().close();

    return id;
}
