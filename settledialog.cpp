#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include "settledialog.h"
#include "ui_settledialog.h"
#include "store.h"
#include "goodsdefine.h"
#include "orderdefine.h"
#include "dboperation.h"
#include "viewgoodsdialog.h"
#include "viewgoodsbinddialog.h"

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
    delete ui;
}

void SettleDialog::on_settleButton_clicked()
{
    int money = 0;
    int size = m_curOrder.size();
    if (size == 0)
    {
        QMessageBox::warning(this, tr("order"), tr("请先选择一个订单."));
        return;
    }
    for (int i = 0; i < size; ++i)
    {
        if (m_curOrder[i]->goods_id.isEmpty() && !m_curOrder[i]->ignore)
        {
            QMessageBox::warning(this, tr("order"), tr("尚有商品未被结算."));
            return;
        }

        float order_price = m_curOrder[i]->price.toFloat();
        float goods_price = m_curOrder[i]->goods->realPrice.toFloat();
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
    int row = ui->orderView->currentIndex().row();
    if (0 > row || row >= m_curOrder.size())
    {
        QMessageBox::warning(this, tr("order"), tr("WTF? the order row is error."));
        return;
    }

    Order* order = m_curOrder[row];
    if (NULL == order)
    {
        QMessageBox::warning(this, tr("order"), tr("WTF? the order is empty."));
        return;
    }

    if (false == order->goods_id.isEmpty())
    {
        QMessageBox::warning(this, tr("order"), tr("the order has been settled."));
        return;
    }
    
    Goods* goods = getGoods();
    if (goods == NULL)
    {
        return;
    }


    if (false == Store::instance()->orderAttachGoods(order, goods))
    {
        QMessageBox::warning(this, tr("order"), tr("attach goods to order failed."));
        return;
    }

    QAbstractItemModel* orderModel = ui->orderView->model();
    QModelIndex orderIndex = orderModel->index(row,0);
    orderModel->setData(orderIndex, QVariant(goods->id));
    order->goods_id = goods->id;
    order->goods = goods;
}

void SettleDialog::on_detachButton_clicked()
{
    int row = ui->orderView->currentIndex().row();
    if (0 > row || row >= m_curOrder.size())
    {
        QMessageBox::warning(this, tr("order"), tr("请选择一个订单."));
        return;
    }

    Order* order = m_curOrder[row];
    if (NULL == order)
    {
        QMessageBox::warning(this, tr("order"), tr("WTF? the order is empty."));
        return;
    }

    if (false == order->goods_id.isEmpty())
    {
        QMessageBox::warning(this, tr("order"), tr("the order has not been settled."));
        return;
    }

    if (false == Store::instance()->orderDetachGoods(order, order->goods_id))
    {
        QMessageBox::warning(this, tr("order"), tr("detach goods from order failed."));
        return;
    }

    QAbstractItemModel* orderModel = ui->orderView->model();
    QModelIndex orderIndex = orderModel->index(row,7);
    orderModel->setData(orderIndex, QVariant(""));
    order->goods_id = "";
    delete order->goods;
    order->goods = NULL;
}

void SettleDialog::init()
{
    setFixedSize(this->width(), this->height());
    ui->orderView->verticalHeader()->hide();
    // ui->orderView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStandardItemModel* orderModel = new QStandardItemModel(0, 8);
    ui->orderView->setModel(orderModel);
    int col = 0;
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("商品ID"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("标题"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("数量"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("买家备注"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("卖家备注"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("用户名"));
    orderModel->setHeaderData(col++, Qt::Horizontal, tr("忽略"));

    ui->orderView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->orderView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_tables = DBOperation::getAllTableName();
    loadOrderDate();
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

void SettleDialog::insertOrderDate(const QString& date)
{
    QString year = date.left(4);
    QString day = date.right(2);
    QString month = date.mid(4, 2);
    m_orderDate[year][month].append(day);
}

void SettleDialog::fillOrderMonthBox(const QString& year)
{
    if (false == m_orderDate.contains(year))
    {
        return;
    }

    ui->orderMonthBox->clear();
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

    ui->orderDayBox->clear();
    auto days = months[month];
    for (int i = 0; i < days.size(); i++)
    {
        ui->orderDayBox->addItem(days[i]);
    }
}

Goods *SettleDialog::getGoods()
{
    ViewGoodsDialog dlg(ViewGoodsDialog::MODE_SETTLE);
    dlg.exec();
    return dlg.selectGoods();
}

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
        model->setItem(i, col++, new QStandardItem(order->title));
        model->setItem(i, col++, new QStandardItem(order->price));
        model->setItem(i, col++, new QStandardItem(order->count));
        model->setItem(i, col++, new QStandardItem(order->user_remark));
        model->setItem(i, col++, new QStandardItem(order->sell_remark));
        model->setItem(i, col++, new QStandardItem(order->user_name));
        order->date = date;
    }
}

void SettleDialog::on_orderView_doubleClicked(const QModelIndex &index)
{
    qDebug() << index.row();
    Goods* goods = getGoods();

    //
    if (goods == NULL)
    {
        return;
    }

    int row = index.row();
    if (row >= m_curOrder.size())
    {
        qDebug() << "error order row";
        return;
    }

    Order* order = m_curOrder[row];
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

    if (false == Store::instance()->orderAttachGoods(order, goods))
    {
        qDebug() << "attach goods to order failed";
        // QMessageBox
        return;
    }

    QAbstractItemModel* orderModel = ui->orderView->model();
    QModelIndex orderIndex = orderModel->index(row,0);
    orderModel->setData(orderIndex, QVariant(goods->id));
    order->goods_id = goods->id;
    order->goods = goods;
}

void SettleDialog::on_viewBindButton_clicked()
{
    int orderRow = ui->orderView->currentIndex().row();
    if (0 > orderRow || orderRow >= m_curOrder.size())
    {
        QMessageBox::warning(this, tr("order"), tr("请选择一个订单."));
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
        return;
    }

    ViewGoodsBindDialog dlg;
    dlg.setOrder(order);
    dlg.load();
    dlg.exec();
    on_orderSearchButton_clicked();
}

void SettleDialog::on_ignoreButton_clicked()
{
    int orderRow = ui->orderView->currentIndex().row();
    if (0 > orderRow || orderRow >= m_curOrder.size())
    {
        QMessageBox::warning(this, tr("order"), tr("请选择一个订单."));
        return;
    }

    Order* order = m_curOrder[orderRow];
    if (NULL == order)
    {
        qDebug() << "error order";
        return;
    }

    order->ignore = !order->ignore;

    QAbstractItemModel* orderModel = ui->orderView->model();
    QModelIndex orderIndex = orderModel->index(orderRow,7);

    if (order->ignore)
    {
        ui->ignoreButton->setText("取消忽略");
        orderModel->setData(orderIndex, QVariant("忽略"));
    }
    else
    {
        ui->ignoreButton->setText("忽略");
        orderModel->setData(orderIndex, QVariant(""));
    }
}

void SettleDialog::on_orderView_clicked(const QModelIndex &index)
{
    int row = index.row();
    if (row < 0 || row >= m_curOrder.size())
    {
        qDebug() << "error order row";
        return;
    }

    Order* order = m_curOrder[row];
    if (NULL == order)
    {
        qDebug() << "error order";
        return;
    }

    if (order->ignore)
    {
        ui->ignoreButton->setText("取消忽略");
    }
    else
    {
        ui->ignoreButton->setText("忽略");
    }
}

void SettleDialog::on_orderYearBox_currentIndexChanged(const QString &year)
{
    fillOrderMonthBox(year);
}

void SettleDialog::on_orderMonthBox_currentIndexChanged(const QString &month)
{
    fillOrderDayBox(month);
}
