#include <QDebug>
#include <QStandardItemModel>
#include "settledialog.h"
#include "ui_settledialog.h"
#include "store.h"
#include "goodsdefine.h"
#include "orderdefine.h"
#include "dboperation.h"

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
//    clearGoods();
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
//    int goodsRow = ui->goodsView->currentIndex().row();
//    if (0 > goodsRow || goodsRow >= m_curGoods.size())
//    {
//        qDebug() << "error goods row";
//        return;
//    }

//    Goods* goods = m_curGoods[goodsRow];
//    if (NULL == goods)
//    {
//        qDebug() << "error goods";
//        return;
//    }
//    if (false != goods->settled)
//    {
//        qDebug() << "goods has been settled";
//        return;
//    }

//    int orderRow = ui->orderView->currentIndex().row();
//    if (0 > orderRow || orderRow >= m_curOrder.size())
//    {
//        qDebug() << "error order row";
//        return;
//    }

//    Order* order = m_curOrder[orderRow];
//    if (NULL == order)
//    {
//        qDebug() << "error order";
//        return;
//    }
//    if (false == order->goods_id.isEmpty())
//    {
//        qDebug() << "order has been settled";
//        return;
//    }

//    if (false == Store::instance()->orderAttachGoods(order, goods))
//    {
//        qDebug() << "attach goods to order failed";
//        // QMessageBox
//        return;
//    }

//    QAbstractItemModel* orderModel = ui->orderView->model();
//    QModelIndex orderIndex = orderModel->index(orderRow,0);
//    orderModel->setData(orderIndex, QVariant(goods->id));
//    order->goods_id = goods->id;

//    QAbstractItemModel* goodsModel = ui->goodsView->model();
//    QModelIndex goodsIndex = goodsModel->index(orderRow,0);
//    goodsModel->setData(goodsIndex, QVariant("是"));
//    goods->settled = true;
}

void SettleDialog::on_detachButton_clicked()
{
//    int orderRow = ui->orderView->currentIndex().row();
//    if (orderRow >= m_curOrder.size())
//    {
//        qDebug() << "error order row";
//        return;
//    }

//    Order* order = m_curOrder[orderRow];
//    if (NULL == order)
//    {
//        qDebug() << "error order";
//        return;
//    }

//    if (false == order->goods_id.isEmpty())
//    {
//        qDebug() << "order has not been settled";
//        return;
//    }

//    Goods* goods = NULL;
//    int goodsRow = -1;
//    for (int i = 0; i < m_curGoods.size(); ++i)
//    {
//        if (NULL == m_curGoods[i])
//        {
//            continue;
//        }
//        if (goods->id == order->goods_id)
//        {
//            goods = m_curGoods[i];
//            goodsRow = i;
//            break;
//        }
//    }

//    if (NULL == goods || 0 > goodsRow)
//    {
//        return;
//    }

//    if (false == Store::instance()->orderDetachGoods(order, goods))
//    {
//        qDebug() << "detach goods from order failed";
//        // QMessageBox
//        return;
//    }

//    QAbstractItemModel* orderModel = ui->orderView->model();
//    QModelIndex orderIndex = orderModel->index(orderRow,7);
//    orderModel->setData(orderIndex, QVariant(""));
//    order->goods_id = "";

//    QAbstractItemModel* goodsModel = ui->orderView->model();
//    QModelIndex goodsIndex = goodsModel->index(goodsRow,4);
//    goodsModel->setData(goodsIndex, QVariant(""));
//    goods->settled = false;
}

void SettleDialog::init()
{
//    QStandardItemModel* goodsModel = new QStandardItemModel(0, 5);
//    ui->goodsView->setModel(goodsModel);
//    int col = 0;
//    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("已结算"));
//    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("商品ID"));
//    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("商品名"));
//    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("价格"));
//    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("属性"));
//    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("数量"));

    QStandardItemModel* orderModel = new QStandardItemModel(0, 8);
    ui->orderView->setModel(orderModel);
    int col = 0;
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("商品ID"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("订单编号"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("标题"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("数量"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("买家备注"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("卖家备注"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("用户名"));

    ui->orderView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->orderView->setEditTriggers(QAbstractItemView::NoEditTriggers);

//    ui->goodsView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->goodsView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_tables = DBOperation::getAllTableName();
    loadOrderDate();
//    loadGoodsDate();
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

//void SettleDialog::clearGoods()
//{
//    int size = m_curGoods.size();
//    for (int i = 0; i < size; ++i)
//    {
//        if (NULL != m_curGoods[i])
//        {
//            delete m_curGoods[i];
//            m_curGoods[i] = NULL;
//        }
//    }
//    m_curGoods.clear();
//    ui->goodsView->reset();
//}

void SettleDialog::loadOrderDate()
{
    for (int i = 0; i < m_tables.size(); ++i)
    {
        const QString& tableName = m_tables[i];
        if (0 == tableName.indexOf("ORDER_20"))
        {
            QString date = tableName.right(8);
            insertOrderDate(date);
        }
    }

    auto it = m_orderDate.begin();
    for (; it != m_orderDate.end(); it++)
    {
        ui->orderYearBox->addItem(it.key());
    }

    fillOrderMonthBox(ui->orderYearBox->currentText());
    fillOrderDayBox(ui->orderMonthBox->currentText());
}

//void SettleDialog::loadGoodsDate()
//{
//    for (int i = 0; i < m_tables.size(); ++i)
//    {
//        const QString& tableName = m_tables[i];
//        if (0 == tableName.indexOf("GOODS_20"))
//        {
//            QString date = tableName.right(8);
//            insertGoodsDate(date);
//        }
//    }

//    auto it = m_goodsDate.begin();
//    for (; it != m_goodsDate.end(); it++)
//    {
//        ui->goodsYearBox->addItem(it.key());
//    }


//    fillGoodsMonthBox(ui->goodsYearBox->currentText());
//    fillGoodsDayBox(ui->goodsMonthBox->currentText());
//}

void SettleDialog::insertOrderDate(const QString& date)
{
    QString year = date.left(4);
    QString day = date.right(2);
    QString month = date.mid(4, 2);
    m_orderDate[year][month].append(day);
}

//void SettleDialog::insertGoodsDate(const QString& date)
//{
//    QString year = date.left(4);
//    QString day = date.right(2);
//    QString month = date.mid(4, 2);
//    m_goodsDate[year][month].append(day);
//}

void SettleDialog::fillOrderMonthBox(const QString& year)
{
    if (false == m_orderDate.contains(year))
    {
        return;
    }

    auto months = m_orderDate[year];
    auto it = months.begin();
    for (; it != months.end(); it++)
    {
        ui->orderMonthBox->addItem(it.key());
    }
}

void SettleDialog::fillOrderDayBox(const QString& month)
{
    auto year = ui->orderYearBox->currentText();
    if (false == m_orderDate.contains(year))
    {
        return;
    }

    auto months = m_orderDate[year];
    if (false == months.contains(month))
    {
        return;
    }

    auto days = months[month];
    for (int i = 0; i < days.size(); i++)
    {
        ui->orderDayBox->addItem(days[i]);
    }
}

//void SettleDialog::fillGoodsMonthBox(const QString& year)
//{
//    if (false == m_goodsDate.contains(year))
//    {
//        return;
//    }

//    auto months = m_goodsDate[year];
//    auto it = months.begin();
//    for (; it != months.end(); it++)
//    {
//        ui->goodsMonthBox->addItem(it.key());
//    }
//}

//void SettleDialog::fillGoodsDayBox(const QString& month)
//{
//    auto year = ui->goodsYearBox->currentText();
//    if (false == m_goodsDate.contains(year))
//    {
//        return;
//    }

//    auto months = m_goodsDate[year];
//    if (false == months.contains(month))
//    {
//        return;
//    }

//    auto days = months[month];
//    for (int i = 0; i < days.size(); i++)
//    {
//        ui->goodsDayBox->addItem(days[i]);
//    }
//}

void SettleDialog::on_orderSearchButton_clicked()
{
    QString date = ui->orderYearBox->currentText() +
                   ui->orderMonthBox->currentText() +
                   ui->orderDayBox->currentText();
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
        model->setItem(i, col++, new QStandardItem(order->goods_id));
        model->setItem(i, col++, new QStandardItem(order->id));
        model->setItem(i, col++, new QStandardItem(order->title));
        model->setItem(i, col++, new QStandardItem(order->price));
        model->setItem(i, col++, new QStandardItem(order->count));
        model->setItem(i, col++, new QStandardItem(order->user_remark));
        model->setItem(i, col++, new QStandardItem(order->sell_remark));
        model->setItem(i, col++, new QStandardItem(order->user_name));
        order->date = date;
    }
}

//void SettleDialog::on_goodsSearchButton_clicked()
//{
//    QString date = ui->goodsYearBox->currentText() +
//                   ui->goodsMonthBox->currentText() +
//                   ui->goodsDayBox->currentText();
//    qDebug() << "goodsDateEdit:" << date;

//    clearGoods();
//    Store::instance()->getGoodsByDate(date, m_curGoods);

//    if (m_curGoods.empty())
//    {
//        qDebug() << "empty goods";
//        return;
//    }

//    QStandardItemModel* model = (QStandardItemModel*)ui->goodsView->model();
//    if (NULL == model)
//    {
//        qDebug() << "empty model";
//        return;
//    }

//    for (int i = 0; i < m_curGoods.size(); ++i)
//    {
//        Goods* goods = m_curGoods[i];
//        int col = 0;
//        model->setItem(i, col++, new QStandardItem(goods->settled ? "是" : "否"));
//        model->setItem(i, col++, new QStandardItem(goods->id));
//        model->setItem(i, col++, new QStandardItem(goods->name));
//        model->setItem(i, col++, new QStandardItem(goods->price));
//        model->setItem(i, col++, new QStandardItem(goods->attribute));
//        model->setItem(i, col++, new QStandardItem(goods->count));
//        goods->date = date;
//    }
//}
