#include "settledialog.h"
#include "ui_settledialog.h"

SettleDialog::SettleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettleDialog)
{
    ui->setupUi(this);
}

SettleDialog::~SettleDialog()
{
    delete ui;
}

void SettleDialog::on_settleButton_clicked()
{

}

void SettleDialog::on_exitButton_clicked()
{
    accept();
}

void SettleDialog::on_attachButton_clicked()
{

}

void SettleDialog::on_detachButton_clicked()
{

}
