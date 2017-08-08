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
    QString invoiceId;
    QString shopName;
    bool settled;
};

#endif // INVOICEDEFINE_H
