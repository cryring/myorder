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
