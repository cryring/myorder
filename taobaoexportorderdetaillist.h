#ifndef TAOBAOEXPORTORDERDETAILLIST_H
#define TAOBAOEXPORTORDERDETAILLIST_H

#include <QMap>
#include <QVector>
#include "orderdefine.h"
#include "csvprocessor.h"

class TaobaoExportOrderDetailList : public CsvProcessor
{
public:
    TaobaoExportOrderDetailList();

    ~TaobaoExportOrderDetailList();

public:
    virtual bool process(const QStringList& fields);

    QVector<TBExportOrderDetail*>* get(const QString& id);

private:
    void clear();

private:
    QMap<QString, QVector<TBExportOrderDetail*> > m_orders;
};

#endif // TAOBAOEXPORTORDERDETAILLIST_H

