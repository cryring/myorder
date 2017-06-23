#include "invoicemanagedialog.h"
#include "ui_invoicemanagedialog.h"
#include "newinvoicedialog.h"
#include "viewinvoicedialog.h"

InvoiceManageDialog::InvoiceManageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvoiceManageDialog)
{
    ui->setupUi(this);
}

InvoiceManageDialog::~InvoiceManageDialog()
{
    delete ui;
}

void InvoiceManageDialog::on_newInvoiceButton_clicked()
{
    NewInvoiceDialog dlg;
    dlg.exec();
}

void InvoiceManageDialog::on_viewInvoiceButton_clicked()
{
    ViewInvoiceDialog dlg;
    dlg.exec();
}
