#include "ordermanagedialog.h"
#include "ui_ordermanagedialog.h"
#include "orderimportdialog.h"
#include "vieworderdialog.h"

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

void OrderManageDialog::on_pushButton_2_clicked()
{
    ViewOrderDialog dlg;
    dlg.exec();
}
