#include <QObject>
#include "taobaoexportorderlist.h"

TaobaoExportOrderList::TaobaoExportOrderList()
{

}

TaobaoExportOrderList::~TaobaoExportOrderList()
{
    clear();
}

bool TaobaoExportOrderList::process(const QStringList& fields)
{
    if (45 != fields.size())
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

    TBExportOrder* order = new TBExportOrder();
    order->id = id;
    order->user_name = QString(fields[1]).replace("\"", "");
    order->user_alipay_id = QString(fields[2]).replace("\"", "");
    order->postage = QString(fields[4]).replace("\"", "");
    m_orders[id] = order;
    return true;
}

TBExportOrder* TaobaoExportOrderList::get(const QString& id)
{
    if (false == m_orders.contains(id))
    {
        return NULL;
    }

    return m_orders[id];
}

void TaobaoExportOrderList::getIds(QVector<QString>& ids)
{
    QMap<QString, TBExportOrder*>::iterator it = m_orders.begin();
    for (; it != m_orders.end(); ++it)
    {
        ids.append(it.key());
    }
}

void TaobaoExportOrderList::clear()
{
    QMap<QString, TBExportOrder*>::iterator it = m_orders.begin();
    for (; it != m_orders.end(); ++it)
    {
        delete it.value();
    }
    m_orders.clear();
}

