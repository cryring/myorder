#include <QtSql>
#include <QtDebug>
#include "store.h"
#include "orderstore.h"
#include "goodsstore.h"

Store::Store()
{
    m_orderStore = new OrderStore();
    m_goodsStore = new GoodsStore();
}

Store::~Store()
{
    delete m_orderStore;
    delete m_goodsStore;
}

bool Store::init(QSqlDatabase* db)
{
    if (false == m_orderStore->init(db))
    {
        return false;
    }
    if (false == m_goodsStore->init(db))
    {
        return false;
    }

    return true;
}

bool Store::insertOrder(const QString &date, Order *order)
{
    if (NULL != m_orderStore)
    {
        return m_orderStore->insert(date, order);
    }
    return false;
}

bool Store::insertGoods(const QString &date, Goods *goods)
{
    if (NULL != m_goodsStore)
    {
        return m_goodsStore->insert(date, goods);
    }
    return false;
}

bool Store::orderAttachGoods(Order* order, Goods* goods)
{
    if (NULL == order || NULL == goods)
    {
        return false;
    }

    order->goods_id = goods->id;
    order->goods_price = goods->price;

    bool ret = false;
    do
    {
        if (false == m_orderStore->attachGoods(order))
        {
            break;
        }

        if (false == m_goodsStore->updateSettle(goods, true))
        {
            m_orderStore->detachGoods(order);
            break;
        }

        ret = true;

    } while (false);

    if (false == ret)
    {
        order->goods_id = "";
        order->goods_price = "";
    }

    return true;
}

bool Store::orderDetachGoods(Order* order, Goods* goods)
{
    if (NULL == order || NULL == goods)
    {
        return false;
    }

    if (false == m_orderStore->detachGoods(order))
    {
        return false;
    }

    if (false == m_goodsStore->updateSettle(goods, false))
    {
        m_orderStore->attachGoods(order);
        return false;
    }

    return true;
}

void Store::getOrderByDate(const QString& date, QVector<Order*>& orders)
{
    if (NULL != m_orderStore)
    {
        m_orderStore->select(date, orders);
    }
}

void Store::getGoodsByDate(const QString& date, QVector<Goods*>& goodss)
{
    if (NULL != m_goodsStore)
    {
        m_goodsStore->select(date, goodss);
    }
}
