#include <QStandardItemModel>
#include "settledialog.h"
#include "ui_settledialog.h"

SettleDialog::SettleDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SettleDialog)
{
    ui->setupUi(this);

    init();
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
    QModelIndex index = ui->goodsView->currentIndex();
}

void SettleDialog::on_detachButton_clicked()
{

}

void SettleDialog::init()
{
    QStandardItemModel* goodsModel = new QStandardItemModel(0, 5);
    ui->goodsView->setModel(goodsModel);
    int col = 0;
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("已结算"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("商品ID"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("商品名"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("属性"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("数量"));

    QStandardItemModel* orderModel = new QStandardItemModel(0, 8);
    ui->orderView->setModel(orderModel);
    col = 0;
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("商品ID"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("订单编号"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("标题"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("数量"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("买家备注"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("卖家备注"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("用户名"));
}

