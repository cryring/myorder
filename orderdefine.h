#ifndef ORDERDEFINE_H
#define ORDERDEFINE_H

#include <QString>

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

struct OrderDetail
{
    QString id;
    QString title;
    QString price;
    QString count;
    QString postage;
    QString total_price;
    QString state;
    QString user_remark;
    QString sell_remark;
    QString user_name;
    QString user_alipay_id;
};

#endif // ORDERDEFINE_H
