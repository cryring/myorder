#include "ordermanagedialog.h"
#include "ui_ordermanagedialog.h"
#include "orderimportdialog.h"

OrderManageDialog::OrderManageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderManageDialog)
{
    ui->setupUi(this);
}

OrderManageDialog::~OrderManageDialog()
{
    delete ui;
}

void OrderManageDialog::on_importTBorderButton_clicked()
{
    OrderImportDialog dlg;
    dlg.exec();
}
