#include "goodsmanagedialog.h"
#include "ui_goodsmanagedialog.h"
#include "goodsimportdialog.h"
#include "viewgoodsdialog.h"

GoodsManageDialog::GoodsManageDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::GoodsManageDialog)
{
    ui->setupUi(this);
}

GoodsManageDialog::~GoodsManageDialog()
{
    delete ui;
}

void GoodsManageDialog::on_newInvoiceButton_clicked()
{
    GoodsImportDialog dlg;
    dlg.exec();
}

void GoodsManageDialog::on_viewInvoiceButton_clicked()
{
    ViewGoodsDialog dlg;
    dlg.exec();
}
