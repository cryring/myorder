#ifndef STORE_H
#define STORE_H

#include <QString>
#include <QVector>
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

    bool insertOrder(Order* order);

    bool insertGoods(Goods* goods);

    bool updateGoods(Goods* goods);

    bool removeGoodsByInvoiceID(const QString& date, const QString& invoiceid);

    bool goodsExist(const QString& id);

    bool orderAttachGoods(Order* order, Goods* goods);

    bool orderDetachGoods(Order* order, const QString& goods_id);

    void getOrderByDate(const QString& date, QVector<Order*>& orders);

    void getGoodsByDate(const QString& date, QVector<Goods*>& goodss);

    Goods* getGoodsByID(const QString& id);

private:
    OrderStore* m_orderStore;

    GoodsStore* m_goodsStore;
};

#endif // STORE_H
