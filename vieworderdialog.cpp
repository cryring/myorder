#include <QDebug>
#include <QStandardItemModel>
#include "vieworderdialog.h"
#include "ui_vieworderdialog.h"
#include "orderdefine.h"
#include "store.h"

ViewOrderDialog::ViewOrderDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ViewOrderDialog)
{
    ui->setupUi(this);
    init();
}

ViewOrderDialog::~ViewOrderDialog()
{
    delete ui;
}

void ViewOrderDialog::on_queryButton_clicked()
{
    QString date = ui->dateEdit->date().toString("yyyyMMdd");
    qDebug() << "dateEdit:" << date;

    clearCurrentOrders();
    Store::instance()->getOrderByDate(date, m_currentOrders);
    if (m_currentOrders.empty())
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

    for (int i = 0; i < m_currentOrders.size(); ++i)
    {
        Order* order = m_currentOrders[i];
        int col = 0;
        model->setItem(i, col++, new QStandardItem(order->id));
        model->setItem(i, col++, new QStandardItem(order->title));
        model->setItem(i, col++, new QStandardItem(order->price));
        model->setItem(i, col++, new QStandardItem(order->count));
        model->setItem(i, col++, new QStandardItem(order->user_remark));
        model->setItem(i, col++, new QStandardItem(order->sell_remark));
        model->setItem(i, col++, new QStandardItem(order->user_name));
    }
}

void ViewOrderDialog::init()
{
    QStandardItemModel* model = new QStandardItemModel(0, 7);
    ui->orderView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("订单编号"));
    model->setHeaderData(col++, Qt::Horizontal, tr("标题"));
    model->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    model->setHeaderData(col++, Qt::Horizontal, tr("数量"));
    model->setHeaderData(col++, Qt::Horizontal, tr("买家备注"));
    model->setHeaderData(col++, Qt::Horizontal, tr("卖家备注"));
    model->setHeaderData(col++, Qt::Horizontal, tr("用户名"));
}

void ViewOrderDialog::clearCurrentOrders()
{
    for (int i = 0; i < m_currentOrders.size(); ++i)
    {
        delete m_currentOrders[i];
        m_currentOrders[i] = NULL;
    }
    m_currentOrders.clear();
}
