#ifndef INVOICESTORE_H
#define INVOICESTORE_H

#include <QMap>
#include <QString>
#include "singleton.h"

struct InvoiceBase
{
    QString date;
    QString id;
    QString cash;
    QString cashRate;
    QString cashChange;
    QString coupon;
    QString couponDiscount;
    QString couponRate;
    QString creditCard;
    QString shopName;
};

class InvoiceStore : public Singleton<InvoiceStore>
{
public:
    InvoiceStore();

    ~InvoiceStore();

public:
    bool init(void);

    bool set(InvoiceBase* ib);

    InvoiceBase* get(const QString& id);

    void getByDate(const QString& date, QVector<InvoiceBase*>& ibs);

    bool del(const QString& id);

private:
    void clear(void);

private:
    QMap<QString, InvoiceBase*> m_ibs;
};

#endif // INVOICESTORE_H
