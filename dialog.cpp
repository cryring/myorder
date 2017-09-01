#include "dialog.h"
#include "ui_dialog.h"
#include "invoicemanagedialog.h"
#include "ordermanagedialog.h"
#include "shopmanagedialog.h"
#include "goodsnamemanagedialog.h"
#include "settledialog.h"

Dialog::Dialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    init();
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
    SettleDialog dlg;
    dlg.exec();
}

void Dialog::on_goodsNameManageButton_clicked()
{
    GoodsNameManageDialog dlg;
    dlg.exec();
}

void Dialog::on_shopManageButton_clicked()
{
    ShopManageDialog dlg;
    dlg.exec();
}

void Dialog::init()
{
    this->setProperty("Form", true);
    this->setProperty("CanMove", true);
}
