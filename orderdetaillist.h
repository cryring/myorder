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

    Order* get(const QString& id);

    void setDate(const QString& date);

    QString date(void) const;

private:
    QMap<QString, Order*> m_orders;

    QString m_date;
};

#endif // ORDERDETAILLIST_H
