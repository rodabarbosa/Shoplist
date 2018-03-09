#include "brandentrywindow.h"
#include "ui_brandentrywindow.h"

BrandEntryWindow::BrandEntryWindow(CommonData &commonData, const int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrandEntryWindow)
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

BrandEntryWindow::~BrandEntryWindow()
{
    delete this->_query;
    delete ui;
}

void BrandEntryWindow::initialize()
{
    this->setLayout(ui->formLayout);
    ui->formLayout->setMargin(15);

    this->_query = new QSqlQuery(this->_commonData->getDBConnection().getConnection());
}

void BrandEntryWindow::on_buttonBox_accepted()
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
}

void BrandEntryWindow::loadFrame()
{
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT id, name FROM brand WHERE id = :id");
    this->_query->bindValue(":id", this->_id);

    if (this->_query->exec()) {
        if (this->_query->next()) {
            ui->edtId->setText(this->_query->value(0).toString());
            ui->edtName->setText(this->_query->value(1).toString());
        }
    }
    this->_commonData->getDBConnection().close();
}

bool BrandEntryWindow::insertEntry()
{
    bool success = false;
    this->_commonData->getDBConnection().open();
    this->_query->prepare("INSERT INTO brand(name) VALUES(:name)");
    this->_query->bindValue(":name", ui->edtName->text());
    success = this->_query->exec();
    if (success) {
        this->_commonData->getDBConnection().setLastInsertedId(this->_query->lastInsertId().toInt());
    }
    return success;
}

bool BrandEntryWindow::updateEntry()
{
    bool success = false;
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("UPDATE brand SET name = :name WHERE id = :id");
    this->_query->bindValue(":name", ui->edtName->text());
    this->_query->bindValue(":id", ui->edtId->text());
    success = this->_query->exec();

    return success;
}
