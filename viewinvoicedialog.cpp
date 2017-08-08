#include "viewinvoicedialog.h"
#include "ui_viewinvoicedialog.h"

ViewInvoiceDialog::ViewInvoiceDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ViewInvoiceDialog)
{
    ui->setupUi(this);
}

ViewInvoiceDialog::~ViewInvoiceDialog()
{
    delete ui;
}
