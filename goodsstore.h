#ifndef GOODSSTORE_H
#define GOODSSTORE_H

#include <QString>
#include "goodsdefine.h"

class QSqlDatabase;

class GoodsStore
{
public:
    GoodsStore();

    ~GoodsStore();

public:
    bool init(QSqlDatabase* db);

    bool insert(const QString& date, Goods* goods);

    void select(const QString& date, QVector<Goods*>& goodss);

    Goods* selectGoodsByID(const QString& goods_id);

    bool updateSettle(const QString& goods_id, bool settled);

private:
    QSqlDatabase* m_db;
};

#endif // GOODSSTORE_H
