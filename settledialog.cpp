#include <QDebug>
#include <QStandardItemModel>
#include "settledialog.h"
#include "ui_settledialog.h"
#include "store.h"
#include "goodsdefine.h"
#include "orderdefine.h"

SettleDialog::SettleDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SettleDialog)
{
    ui->setupUi(this);

    init();
}

SettleDialog::~SettleDialog()
{
    clearOrder();
    clearGoods();
    delete ui;
}

void SettleDialog::on_settleButton_clicked()
{
    int money = 0;
    int size = m_curOrder.size();
    for (int i = 0; i < size; ++i)
    {
        if (m_curOrder[i]->goods_id.isEmpty())
        {
            // TODO:
            return;
        }

        float order_price = m_curOrder[i]->price.toFloat();
        float goods_price = m_curOrder[i]->goods_price.toFloat();
        money += order_price - goods_price;
    }
    ui->profitEdit->setText(QString::number(money));
}

void SettleDialog::on_exitButton_clicked()
{
    accept();
}

void SettleDialog::on_attachButton_clicked()
{
    int goodsRow = ui->goodsView->currentIndex().row();
    if (goodsRow >= m_curGoods.size())
    {
        qDebug() << "error goods row";
        return;
    }

    Goods* goods = m_curGoods[goodsRow];
    if (NULL == goods)
    {
        qDebug() << "error goods";
        return;
    }
    if (false != goods->settled)
    {
        qDebug() << "goods has been settled";
        return;
    }

    int orderRow = ui->orderView->currentIndex().row();
    if (orderRow >= m_curOrder.size())
    {
        qDebug() << "error order row";
        return;
    }

    Order* order = m_curOrder[orderRow];
    if (NULL == order)
    {
        qDebug() << "error order";
        return;
    }
    if (false != order->goods_id.isEmpty())
    {
        qDebug() << "order has been settled";
        return;
    }

    if (false == Store::instance()->orderAttachGoods(order, goods))
    {
        qDebug() << "attach goods to order failed";
        // QMessageBox
        return;
    }

    QAbstractItemModel* orderModel = ui->orderView->model();
    QModelIndex orderIndex = orderModel->index(orderRow,7);
    orderModel->setData(orderIndex, QVariant(goods->id));
    order->goods_id = goods->id;

    QAbstractItemModel* goodsModel = ui->orderView->model();
    QModelIndex goodsIndex = goodsModel->index(orderRow,4);
    goodsModel->setData(goodsIndex, QVariant("done"));
    goods->settled = true;
}

void SettleDialog::on_detachButton_clicked()
{
    int orderRow = ui->orderView->currentIndex().row();
    if (orderRow >= m_curOrder.size())
    {
        qDebug() << "error order row";
        return;
    }

    Order* order = m_curOrder[orderRow];
    if (NULL == order)
    {
        qDebug() << "error order";
        return;
    }

    if (false == order->goods_id.isEmpty())
    {
        qDebug() << "order has not been settled";
        return;
    }

    Goods* goods = NULL;
    int goodsRow = -1;
    for (int i = 0; i < m_curGoods.size(); ++i)
    {
        if (NULL == m_curGoods[i])
        {
            continue;
        }
        if (goods->id == order->goods_id)
        {
            goods = m_curGoods[i];
            goodsRow = i;
            break;
        }
    }

    if (NULL == goods || 0 > goodsRow)
    {
        return;
    }

    if (false == Store::instance()->orderDetachGoods(order, goods))
    {
        qDebug() << "detach goods from order failed";
        // QMessageBox
        return;
    }

    QAbstractItemModel* orderModel = ui->orderView->model();
    QModelIndex orderIndex = orderModel->index(orderRow,7);
    orderModel->setData(orderIndex, QVariant(""));
    order->goods_id = "";

    QAbstractItemModel* goodsModel = ui->orderView->model();
    QModelIndex goodsIndex = goodsModel->index(goodsRow,4);
    goodsModel->setData(goodsIndex, QVariant(""));
    goods->settled = false;
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

void SettleDialog::clearOrder()
{
    int size = m_curOrder.size();
    for (int i = 0; i < size; ++i)
    {
        if (NULL != m_curOrder[i])
        {
            delete m_curOrder[i];
            m_curOrder[i] = NULL;
        }
    }
    m_curOrder.clear();
    ui->orderView->reset();
}

void SettleDialog::clearGoods()
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


void SettleDialog::on_orderSearchButton_clicked()
{
    QString date = ui->orderDateEdit->date().toString("yyyyMMdd");
    qDebug() << "orderDateEdit:" << date;

    clearOrder();
    Store::instance()->getOrderByDate(date, m_curOrder);

    if (m_curOrder.empty())
    {
        qDebug() << "empty orders";
        return;
    }

    QStandardItemModel* model = (QStandardItemModel*)ui->orderView->model();
    if (NULL == model)
    {
        qDebug() << "empty model";
        return;
    }

    for (int i = 0; i < m_curOrder.size(); ++i)
    {
        Order* order = m_curOrder[i];
        int col = 0;
        model->setItem(i, col++, new QStandardItem(order->id));
        model->setItem(i, col++, new QStandardItem(order->title));
        model->setItem(i, col++, new QStandardItem(order->price));
        model->setItem(i, col++, new QStandardItem(order->count));
        model->setItem(i, col++, new QStandardItem(order->user_remark));
        model->setItem(i, col++, new QStandardItem(order->sell_remark));
        model->setItem(i, col++, new QStandardItem(order->user_name));
        model->setItem(i, col++, new QStandardItem(order->goods_id));
    }
}

void SettleDialog::on_goodsSearchButton_clicked()
{
    QString date = ui->goodsDateEdit->date().toString("yyyyMMdd");
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
