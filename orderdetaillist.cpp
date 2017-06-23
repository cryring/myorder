#include <QDebug>
#include "orderdetaillist.h"
#include "taobaoexportorderdetaillist.h"
#include "taobaoexportorderlist.h"

OrderDetailList::OrderDetailList()
{

}

bool OrderDetailList::merge(TaobaoExportOrderList *order_list, TaobaoExportOrderDetailList *order_detail_list)
{
    if (NULL == order_detail_list || NULL == order_list)
    {
        qDebug() << "OrderDetailList::merge Error 1";
        return false;
    }

    QVector<QString> ids;
    order_list->getIds(ids);
    int size = ids.size();
    for (int i = 0; i < size; ++i)
    {
        const QString& id = ids[i];
        QVector<TBExportOrderDetail*>* details = order_detail_list->get(id);
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

        TBExportOrder* tbOrder = order_list->get(id);
        if (NULL == tbOrder)
        {
            // TODO:
            qDebug() << "OrderDetailList::merge Error 4";
            return false;
        }

        int size = details->size();
        for (int i = 0; i < size; ++i)
        {
            OrderDetail* detail = new OrderDetail();
            detail->id = id;
            detail->title = details->at(i)->title;
            detail->price = details->at(i)->price;
            detail->count = details->at(i)->count;
            detail->postage = tbOrder->postage;
            detail->total_price = detail->price;
            detail->state = details->at(i)->state;
            detail->user_remark = details->at(i)->remark;
            detail->sell_remark = tbOrder->sell_remark;
            detail->user_name = tbOrder->user_name;
            detail->user_alipay_id = tbOrder->user_alipay_id;
            m_orders[id].append(detail);
        }
    }
    return true;
}

void OrderDetailList::getIds(QVector<QString> &ids)
{
    QMap<QString, QVector<OrderDetail*> >::iterator it = m_orders.begin();
    for (; it != m_orders.end(); ++it)
    {
        ids.append(it.key());
    }
}

QVector<OrderDetail*>* OrderDetailList::get(const QString &id)
{
    if (false == m_orders.contains(id))
    {
        return NULL;
    }

    return &m_orders[id];
}
