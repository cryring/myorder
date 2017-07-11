#ifndef INVOICEDEFINE_H
#define INVOICEDEFINE_H

#include <QString>

struct Goods
{
    QString invoiceid;
    QString shopName;
    QString name;
    QString price;
    QString attribute;
    QString count;
    QString currency;      // 货币
    QString paymentMethod; // 支付方式
    QString exchangeRate;  // 汇率
    QString discount;      // 折扣
    QString rebate;        // 返点
};

#endif // INVOICEDEFINE_H
