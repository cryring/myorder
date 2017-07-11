#include <QDebug>
#include "orderdetaillist.h"
#include "taobaoexportorderdetaillist.h"
#include "taobaoexportorderlist.h"

OrderDetailList::OrderDetailList()
:m_date("")
{

}

bool OrderDetailList::merge(TaobaoExportOrderList *orderList, TaobaoExportOrderDetailList *orderDetailList)
{
    if (NULL == orderDetailList || NULL == orderList)
    {
        qDebug() << "OrderDetailList::merge Error 1";
        return false;
    }

    QVector<QString> ids;
    orderList->getIds(ids);
    int size = ids.size();
    for (int i = 0; i < size; ++i)
    {
        const QString& id = ids[i];
        QVector<TBExportOrderDetail*>* details = orderDetailList->get(id);
        if (NULL == details)
        {
            // TODO:
            qDebug() << "OrderDetailList::merge Error 2, id not find: " << id;
            continue;
        }
        if (details->empty())
        {
            // TODO:
            qDebug() << "OrderDetailList::merge Error 3";
            return false;
        }

        TBExportOrder* tbOrder = orderList->get(id);
        if (NULL == tbOrder)
        {
            // TODO:
            qDebug() << "OrderDetailList::merge Error 4";
            return false;
        }

        int size = details->size();
        for (int i = 0; i < size; ++i)
        {
            Order* detail = new Order();
            detail->id = id;
            detail->title = details->at(i)->title;
            detail->price = details->at(i)->price;
            detail->count = details->at(i)->count;
            detail->state = details->at(i)->state;
            detail->user_remark = details->at(i)->remark;
            detail->sell_remark = tbOrder->sell_remark;
            detail->user_name = tbOrder->user_name;
            m_orders[id] = detail;
        }
    }
    return true;
}

void OrderDetailList::getIds(QVector<QString> &ids)
{
    QMap<QString, Order*>::iterator it = m_orders.begin();
    for (; it != m_orders.end(); ++it)
    {
        ids.append(it.key());
    }
}

Order* OrderDetailList::get(const QString &id)
{
    if (false == m_orders.contains(id))
    {
        return NULL;
    }

    return m_orders[id];
}

void OrderDetailList::setDate(const QString &date)
{
    m_date = date;
}

QString OrderDetailList::date() const
{
    return m_date;
}
