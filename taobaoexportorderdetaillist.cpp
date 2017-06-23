#include <QObject>
#include <QDebug>
#include "taobaoexportorderdetaillist.h"

TaobaoExportOrderDetailList::TaobaoExportOrderDetailList()
{

}

TaobaoExportOrderDetailList::~TaobaoExportOrderDetailList()
{
    clear();
}

bool TaobaoExportOrderDetailList::process(const QStringList& fields)
{
    if (11 != fields.size())
    {
        return false;
    }

    QString id = fields[0];
    id.replace("=", "");
    id.replace("\"", "");
    if (17 != id.length())
    {
        return false;
    }

//    qDebug() << "Process Detail: " << fields;
    TBExportOrderDetail* order_detail = new TBExportOrderDetail();
    order_detail->id = id;
    order_detail->title = QString(fields[1]).replace("\"", "");
    order_detail->price = QString(fields[2]).replace("\"", "");
    order_detail->count = QString(fields[3]).replace("\"", "");
    order_detail->attribute = QString(fields[5]).replace("\"", "");
    order_detail->remark = QString(fields[7]).replace("\"", "");
    order_detail->state = QString(fields[8]).replace("\"", "");
    m_orders[id].append(order_detail);
    return true;
}

QVector<TBExportOrderDetail*>* TaobaoExportOrderDetailList::get(const QString &id)
{
    if (false == m_orders.contains(id))
    {
        return NULL;
    }

    return &m_orders[id];
}

void TaobaoExportOrderDetailList::clear()
{
    QMap<QString, QVector<TBExportOrderDetail*> >::iterator it = m_orders.begin();
    for (; it != m_orders.end(); ++it)
    {
        const QVector<TBExportOrderDetail*>& arr = it.value();
        int size = arr.size();
        for (int i = 0; i < size; ++i)
        {
            delete arr[i];
        }
    }
    m_orders.clear();
}
