#include "supermarketentrywindow.h"
#include "ui_supermarketentrywindow.h"

SupermarketEntryWindow::SupermarketEntryWindow(CommonData &commonData, const int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SupermarketEntryWindow)
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

SupermarketEntryWindow::~SupermarketEntryWindow()
{
    delete this->_query;
    delete ui;
}

void SupermarketEntryWindow::initialize()
{
    this->setLayout(ui->formLayout);
    ui->formLayout->setMargin(15);

    this->_query = new QSqlQuery(this->_commonData->getDBConnection().getConnection());

}

void SupermarketEntryWindow::loadFrame()
{
    this->_commonData->getDBConnection().open();
    this->_query->prepare("SELECT id, name, active FROM supermarket WHERE id = :id");
    this->_query->bindValue(":id", this->_id);

    if (this->_query->exec()) {
        if (this->_query->next()) {
            ui->edtId->setText(this->_query->value(0).toString());
            ui->edtName->setText(this->_query->value(1).toString());
            ui->chkActive->setChecked(this->_query->value(2).toBool());
        }
    }
    this->_commonData->getDBConnection().close();
}

bool SupermarketEntryWindow::insertEntry()
{
    bool success = false;
    this->_commonData->getDBConnection().open();
    this->_query->prepare("INSERT INTO supermarket(name, active) VALUES(:name, :active)");
    this->_query->bindValue(":name", ui->edtName->text());
    this->_query->bindValue(":active", ui->chkActive->isChecked());
    success = this->_query->exec();
    if (success) {
        int id = this->_query->lastInsertId().toInt();
        this->_commonData->getDBConnection().setLastInsertedId(id);
        this->_commonData->setSelectedSupermarketId(id);
    }
    return success;
}

bool SupermarketEntryWindow::updateEntry()
{
    bool success = false;
    this->_query->clear();
    this->_commonData->getDBConnection().open();
    this->_query->prepare("UPDATE supermarket SET name = :name, active = :active WHERE id = :id");
    this->_query->bindValue(":name", ui->edtName->text());
    this->_query->bindValue(":active", ui->chkActive->isChecked());
    this->_query->bindValue(":id", ui->edtId->text());
    success = this->_query->exec();

    return success;
}

void SupermarketEntryWindow::on_buttonBox_accepted()
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
