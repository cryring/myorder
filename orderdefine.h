#ifndef ORDERDEFINE_H
#define ORDERDEFINE_H

#include <QString>
#include "goodsdefine.h"

struct TBExportOrder
{
    QString id;
    QString user_name;
    QString user_alipay_id;
    QString price;
    QString postage;
    QString total_price;
    QString state;
    QString user_remark;
    QString sell_remark;
    QString title;
};

struct TBExportOrderDetail
{
    QString id;
    QString title;
    QString price;
    QString count;
    QString attribute;
    QString remark;
    QString state;
};

struct Order
{
    // base
    QString date;
    QString id;
    QString title;
    QString price;
    QString count;
    QString state;
    QString user_remark;
    QString sell_remark;
    QString user_name;

    // goods
    QString goods_id;

    // status
    bool ignore;
};

#endif // ORDERDEFINE_H
