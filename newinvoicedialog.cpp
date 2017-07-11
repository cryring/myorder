#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include "newinvoicedialog.h"
#include "ui_newinvoicedialog.h"
#include "store.h"

NewInvoiceDialog::NewInvoiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewInvoiceDialog)
{
    ui->setupUi(this);

    init();
}

NewInvoiceDialog::~NewInvoiceDialog()
{
    delete ui;
}

void NewInvoiceDialog::on_addGoodsButton_clicked()
{
    QString count = ui->goodsCountEdit->text();
    if ("" == count)
    {
        // TODO: Message box
        return;
    }

    QString price = ui->goodsPriceEdit->text();
    if ("" == price)
    {
        // TODO: Message Box
        return;
    }

    QString id = ui->idEdit->text();
    if ("" == id)
    {
        // TODO: Message Box
        return;
    }

    QString name = ui->goodsNameEdit->text();
    if ("" == name)
    {
        // TODO: Message Box
        return;
    }

    QStandardItemModel* model = (QStandardItemModel*)ui->invoiceView->model();
    if (NULL == model)
    {
        // TODO: Message Box
        return;
    }

    int col = 0;
    int row = model->rowCount();
    model->setItem(row, col++, new QStandardItem(name));
    model->setItem(row, col++, new QStandardItem(price));
    model->setItem(row, col++, new QStandardItem(count));

}

void NewInvoiceDialog::on_editGoodsButton_clicked()
{

}

void NewInvoiceDialog::on_delGoodsButton_clicked()
{

}

void NewInvoiceDialog::on_shopNameBox_activated(const QString &name)
{
    qDebug() << "Combo:" << name;
    if ("其它" == name)
    {
        ui->shopNameEdit->setText("");
        ui->shopNameEdit->setEnabled(true);
    }
    else
    {
        ui->shopNameEdit->setText(name);
        ui->shopNameEdit->setEnabled(false);
    }
}

void NewInvoiceDialog::on_saveButton_clicked()
{

}

void NewInvoiceDialog::on_savedGoodsNameBox_activated(const QString &name)
{
    qDebug() << "Combo:" << name;
    if ("自定义" == name)
    {
        ui->goodsNameEdit->setText("");
        ui->goodsNameEdit->setEnabled(true);
    }
    else
    {
        ui->goodsNameEdit->setText(name);
        ui->goodsNameEdit->setEnabled(false);
    }
}

void NewInvoiceDialog::init()
{
    ui->shopNameBox->addItem("新罗免税店");
    ui->shopNameBox->addItem("新世界免税店");
    ui->shopNameBox->addItem("乐天免税店");
    ui->shopNameBox->addItem("都塔免税店");
    ui->shopNameBox->addItem("其它");

    ui->currencyBox->addItem("美元");
    ui->currencyBox->addItem("韩元");
    ui->currencyBox->addItem("人民币");

    ui->savedGoodsNameBox->addItem("自定义");
    ui->savedGoodsNameBox->addItem("兰蔻粉水");
    ui->savedGoodsNameBox->addItem("兰蔻小黑瓶");

    QStandardItemModel* model = new QStandardItemModel(0,3);
    ui->invoiceView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("商品名"));
    model->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    model->setHeaderData(col++, Qt::Horizontal, tr("数量"));
}

