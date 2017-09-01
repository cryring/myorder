#include <QtDebug>
#include <QStandardItemModel>
#include "viewinvoicedialog.h"
#include "ui_viewinvoicedialog.h"
#include "store.h"
#include "goodsdefine.h"

ViewInvoiceDialog::ViewInvoiceDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ViewInvoiceDialog)
{
    ui->setupUi(this);

    init();
}

ViewInvoiceDialog::~ViewInvoiceDialog()
{
    clearGoods();
    delete ui;
}

void ViewInvoiceDialog::on_queryGoodsButton_clicked()
{
    QString date = ui->dateEdit->date().toString("yyyyMMdd");
    qDebug() << "goodsDateEdit:" << date;

    clearGoods();
    Store::instance()->getGoodsByDate(date, m_curGoods);

    if (m_curGoods.empty())
    {
        qDebug() << "empty goods";
        return;
    }

    QStandardItemModel* model = (QStandardItemModel*)ui->goodsView->model();
    if (NULL == model)
    {
        qDebug() << "empty model";
        return;
    }

    for (int i = 0; i < m_curGoods.size(); ++i)
    {
        Goods* goods = m_curGoods[i];
        int col = 0;
        model->setItem(i, col++, new QStandardItem(goods->settled));
        model->setItem(i, col++, new QStandardItem(goods->id));
        model->setItem(i, col++, new QStandardItem(goods->name));
        model->setItem(i, col++, new QStandardItem(goods->price));
        model->setItem(i, col++, new QStandardItem(goods->attribute));
        model->setItem(i, col++, new QStandardItem(goods->count));
    }
}

void ViewInvoiceDialog::init()
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
}

void ViewInvoiceDialog::clearGoods()
{
    int size = m_curGoods.size();
    for (int i = 0; i < size; ++i)
    {
        if (NULL != m_curGoods[i])
        {
            delete m_curGoods[i];
            m_curGoods[i] = NULL;
        }
    }
    m_curGoods.clear();
    ui->goodsView->reset();
}
