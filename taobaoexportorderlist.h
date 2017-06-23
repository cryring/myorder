#ifndef TAOBAOEXPORTORDERLIST_H
#define TAOBAOEXPORTORDERLIST_H

#include <QMap>
#include <QVector>
#include "orderdefine.h"
#include "csvprocessor.h"

class TaobaoExportOrderList : public CsvProcessor
{
public:
    TaobaoExportOrderList();

    ~TaobaoExportOrderList();

public:
    virtual bool process(const QStringList& fields);

    TBExportOrder* get(const QString& id);

    void getIds(QVector<QString>& ids);

private:
    void clear();

private:
    QMap<QString, TBExportOrder*> m_orders;
};

#endif // TAOBAOEXPORTORDERLIST_H
