#ifndef INVOICEDEFINE_H
#define INVOICEDEFINE_H

#include <QString>

struct GoodsAttribute
{
    QString name;
    QString price;
    QString attribute;
    QString count;
};

struct Goods : public GoodsAttribute
{
    QString id;
    QString invoiceId;
    QString shopName;
    QString date;
    int settled;
};

#endif // INVOICEDEFINE_H
