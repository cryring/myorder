#ifndef INVOICE_H
#define INVOICE_H

#include <QVector>
#include "commondefine.h"
#include "invoicedefine.h"

class Invoice
{
public:
    Invoice();

    ~Invoice();

public:
    void addGoods(Goods* goods);

private:
    void clear();

private:
    QString m_id;
    QString m_shopName;
    QString m_date;
    QString m_exchangeRate;
    QString m_taxFree;
    QString m_rebate;
    CurrencyType m_currency;
    QVector<Goods*> m_goods;
};

#endif // INVOICE_H
