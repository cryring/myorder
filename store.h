#ifndef STORE_H
#define STORE_H

#include <QString>
#include "singleton.h"

class QSqlDatabase;
class OrderStore;
class GoodsStore;
struct Goods;
struct Order;

class Store : public Singleton<Store>
{
public:
    Store();

    ~Store();

public:
    bool init(QSqlDatabase* db);

    bool insertOrder(const QString& date, Order* order);

    bool insertGoods(const QString& date, Goods* goods);

    void getOrderByDate();

    void getGoodsByDate();

private:
    OrderStore* m_orderStore;

    GoodsStore* m_goodsStore;
};

#endif // STORE_H
