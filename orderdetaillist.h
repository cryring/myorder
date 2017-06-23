#ifndef ORDERDETAILLIST_H
#define ORDERDETAILLIST_H

#include <QMap>
#include <QVector>
#include <QString>
#include "orderdefine.h"

class TaobaoExportOrderList;
class TaobaoExportOrderDetailList;

class OrderDetailList
{
public:
    OrderDetailList();

public:
    bool merge(TaobaoExportOrderList* order_list, TaobaoExportOrderDetailList* order_detail_list);

    void getIds(QVector<QString>& ids);

    QVector<OrderDetail*>* get(const QString& id);

private:
    QMap<QString, QVector<OrderDetail*> > m_orders;
};

#endif // ORDERDETAILLIST_H
