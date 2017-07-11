#include "dialog.h"
#include "ui_dialog.h"
#include "invoicemanagedialog.h"
#include "ordermanagedialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_invoiceManageButton_clicked()
{
    InvoiceManageDialog dlg;
    dlg.exec();
}

void Dialog::on_orderManageButton_clicked()
{
    OrderManageDialog dlg;
    dlg.exec();
}

void Dialog::on_settleButton_clicked()
{

}
