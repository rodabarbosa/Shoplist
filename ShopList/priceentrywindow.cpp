#include "priceentrywindow.h"
#include "ui_priceentrywindow.h"

PriceEntryWindow::PriceEntryWindow(CommonData &commonData, const int productId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PriceEntryWindow)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_DeleteOnClose);
    this->_commonData = &commonData;
    this->_productId = productId;

    this->initialize();
    this->loadProductName();
    this->loadBrandCombo();
    this->loadSupermarketCombo();
}

PriceEntryWindow::~PriceEntryWindow()
{
    delete this->_brandModel;
    delete this->_supermarketModel;
    delete this->_query;
    delete ui;
}

void PriceEntryWindow::initialize()
{
    this->setLayout(ui->formLayout);
    ui->formLayout->setMargin(15);

    this->_query = new QSqlQuery(this->_commonData->getDBConnection().getConnection());
    this->_brandModel = new QSqlQueryModel(ui->cbxBrand);
    this->_supermarketModel = new QSqlQueryModel(ui->cbxSupermarket);

    ui->dteDate->setDate(this->_commonData->getLastSelectedDate());
    if (this->_commonData->getLastSelectedSupermarketId() > 0) {
        this->selectInformedSupermarket(this->_commonData->getLastSelectedSupermarketId());
    }
}

void PriceEntryWindow::loadProductName()
{
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT name FROM product WHERE id = :id");
    this->_query->bindValue(":id", this->_productId);
    if (this->_query->exec()) {
        if (this->_query->next()) {
            ui->edtName->setText(this->_query->value(0).toString());
        }
    }
    this->_commonData->getDBConnection().close();
}

void PriceEntryWindow::loadPriceData()
{
    int brandId = 0;
    int supermarketId = 0;

    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT id, value, date, brand_id, supermarket_id, description FROM price WHERE id = :id");
    this->_query->bindValue(":id", this->_id);
    if (this->_query->exec()) {
        if (this->_query->next()) {
            ui->edtId->setText(this->_query->value(0).toString());
            ui->spbValue->setValue(this->_query->value(1).toDouble());
            ui->dteDate->setDate(this->_query->value(2).toDate());
            brandId = this->_query->value(3).toInt();
            supermarketId = this->_query->value(4).toInt();
            ui->txtDescription->setPlainText(this->_query->value(5).toString());
        }
    }
    this->_commonData->getDBConnection().close();
    this->selectInformedBrand(brandId);
    this->selectInformedSupermarket(supermarketId);
}

void PriceEntryWindow::loadBrandCombo()
{
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT name FROM brand ORDER BY name");
    this->_query->exec();
    this->_brandModel->setQuery(*this->_query);
    ui->cbxBrand->setModel(this->_brandModel);
    this->_commonData->getDBConnection().close();
}

void PriceEntryWindow::loadSupermarketCombo()
{
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT name, id FROM category ORDER BY name");
    this->_query->exec();
    this->_supermarketModel->setQuery(*this->_query);
    ui->cbxSupermarket->setModel(this->_supermarketModel);
    this->_commonData->getDBConnection().close();
}

void PriceEntryWindow::selectInformedBrand(const int id)
{
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT name FROM brand WHERE id = :id");
    this->_query->bindValue(":id", id);
    if (this->_query->exec()) {
        if (this->_query->next()) {
            QString name = this->_query->value(0).toString();
            for (int i = 0; i < ui->cbxBrand->count(); i++) {
                if (ui->cbxBrand->itemText(i) == name) {
                    ui->cbxBrand->setCurrentIndex(i);
                    break;
                }
            }
        }
    }
    this->_commonData->getDBConnection().close();
}

void PriceEntryWindow::selectInformedSupermarket(const int id)
{
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT name FROM supermarket WHERE id = :id");
    this->_query->bindValue(":id", id);
    if (this->_query->exec()) {
        if (this->_query->next()) {
            QString name = this->_query->value(0).toString();
            for (int i = 0; i < ui->cbxSupermarket->count(); i++) {
                if (ui->cbxSupermarket->itemText(i) == name) {
                    ui->cbxSupermarket->setCurrentIndex(i);
                    break;
                }
            }
        }
    }
    this->_commonData->getDBConnection().close();
}

void PriceEntryWindow::setPriceId(const int id)
{
    this->_id = id;
    this->loadPriceData();
}

bool PriceEntryWindow::insertEntry()
{
    this->_commonData->setSelectedDate(ui->dteDate->date());
    this->_commonData->setSelectedSupermarketId(this->getSelectedSupermarketId());
    int brandId = this->getSelectedBrandId();

    bool success = false;
    this->_commonData->getDBConnection().open();
    this->_query->prepare("INSERT INTO price(value, date, product_id, brand_id, supermarket_id, description) VALUES(:value, :date, :product_id, :brand_id, :supermarket_id, :description)");
    this->_query->bindValue(":value", ui->spbValue->value());
    this->_query->bindValue(":date", this->_commonData->getLastSelectedDate());
    this->_query->bindValue(":product_id", this->_productId);
    this->_query->bindValue(":brand_id", brandId);
    this->_query->bindValue(":supermarket_id", this->_commonData->getLastSelectedSupermarketId());
    this->_query->bindValue(":description", ui->txtDescription->document()->toPlainText());
    success = this->_query->exec();
    if (success) {
        this->_commonData->getDBConnection().setLastInsertedId(this->_query->lastInsertId().toInt());
    }
    return success;
}

bool PriceEntryWindow::updateEntry()
{
    this->_commonData->setSelectedDate(ui->dteDate->date());
    this->_commonData->setSelectedSupermarketId(this->getSelectedSupermarketId());
    int brandId = this->getSelectedBrandId();

    bool success = false;
    this->_commonData->getDBConnection().open();
    this->_query->prepare("UPDATE price SET value = :value, date = :date, product_id = :product_id, brand_id = :brand_id, supermarket_id = :supermarket_id, description = :description WHERE id = :id");
    this->_query->bindValue(":value", ui->spbValue->value());
    this->_query->bindValue(":date", this->_commonData->getLastSelectedDate());
    this->_query->bindValue(":product_id", this->_productId);
    this->_query->bindValue(":brand_id", brandId);
    this->_query->bindValue(":supermarket_id", this->_commonData->getLastSelectedSupermarketId());
    this->_query->bindValue(":description", ui->txtDescription->document()->toPlainText());
    this->_query->bindValue(":id", this->_id);
    success = this->_query->exec();

    return success;

}

int PriceEntryWindow::getSelectedBrandId()
{
    int id = 0;
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT id FROM brand WHERE name = :name");
    this->_query->bindValue(":name", ui->cbxBrand->currentText());
    if (this->_query->exec()) {
        if (this->_query->next()) {
          id= this->_query->value(0).toInt();
        }
    }
    this->_commonData->getDBConnection().close();
    return id;
}

int PriceEntryWindow::getSelectedSupermarketId()
{
    int id = 0;
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT id FROM supermarket WHERE name = :name");
    this->_query->bindValue(":name", ui->cbxSupermarket->currentText());
    if (this->_query->exec()) {
        if (this->_query->next()) {
          id= this->_query->value(0).toInt();
        }
    }
    this->_commonData->getDBConnection().close();
    return id;
}

void PriceEntryWindow::on_buttonBox_accepted()
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

void PriceEntryWindow::on_btnBrand_clicked()
{
    BrandEntryWindow *w = new BrandEntryWindow(*this->_commonData, 0, this);
    w->exec();
    if (w->result()) {
        this->selectInformedBrand(this->_commonData->getDBConnection().getLastInsertedId());
    }
    delete w;
}

void PriceEntryWindow::on_btnSupermarket_clicked()
{
    SupermarketEntryWindow *w = new SupermarketEntryWindow(*this->_commonData, 0, this);
    w->exec();
    if (w->result()) {
        this->selectInformedBrand(this->_commonData->getDBConnection().getLastInsertedId());
    }
    delete w;
}
