#ifndef GOODSSTORE_H
#define GOODSSTORE_H

#include <QString>
#include "invoicedefine.h"

class QSqlDatabase;

class GoodsStore
{
public:
    GoodsStore();

    ~GoodsStore();

public:
    bool init(QSqlDatabase* db);

    bool insert(const QString& date, Goods* goods);

private:
    QSqlDatabase* m_db;
};

#endif // GOODSSTORE_H
