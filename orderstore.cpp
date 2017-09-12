#include <QDebug>
#include <QtSql>
#include "orderstore.h"

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS ORDER_%1(" \
                                "ID VARCHAR PRIMARY KEY NOT NULL," \
                                "TITLE VARCHAR NOT NULL," \
                                "PRICE VARCHAR NOT NULL," \
                                "COUNT VARCHAR NOT NULL," \
                                "USER_REMARK VARCHAR," \
                                "SELL_REMARK VARCHAR," \
                                "USER_NAME VARCHAR NOT NULL," \
                                "GOODS_ID VARCHAR," \
                                "GOODS_PRICE VARCHAR)";

static const char* kInsertSQL = "INSERT INTO ORDER_%1(ID,TITLE,PRICE,COUNT,USER_REMARK,SELL_REMARK,USER_NAME) VALUES(?,?,?,?,?,?,?)";

static const char* kSelectByDateSQL = "SELECT * FROM ORDER_%1";

static const char* kUpdateSettledSQL = "UPDATE ORDER_%1 SET GOODS_ID=?,GOODS_PRICE=? WHERE ID=?";

OrderStore::OrderStore()
    : m_db(NULL)
{

}

OrderStore::~OrderStore()
{

}

bool OrderStore::init(QSqlDatabase* db)
{
    m_db = db;
    return true;
}

bool OrderStore::insert(const QString& date, Order* order)
{
    if (NULL == m_db || NULL == order)
    {
        return false;
    }

    QString createSql = QString(kCreateSQL).arg(date);
    QSqlQuery query;
    query.prepare(createSql);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    QString insertSql = QString(kInsertSQL).arg(date);
    query.prepare(insertSql);
    query.addBindValue(order->id);
    query.addBindValue(order->title);
    query.addBindValue(order->price);
    query.addBindValue(order->count);
    query.addBindValue(order->user_remark);
    query.addBindValue(order->sell_remark);
    query.addBindValue(order->user_name);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

void OrderStore::select(const QString& date, QVector<Order*>& orders)
{
    QString sql = QString(kSelectByDateSQL).arg(date);
    QSqlQuery query;
    query.prepare(sql);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return;
    }

    int idNo = query.record().indexOf("ID");
    int titleNo = query.record().indexOf("TITLE");
    int priceNo = query.record().indexOf("PRICE");
    int countNo = query.record().indexOf("COUNT");
    int urNo = query.record().indexOf("USER_REMARK");
    int srNo = query.record().indexOf("SELL_REMARK");
    int unameNo = query.record().indexOf("USER_NAME");
    int gidNo = query.record().indexOf("GOODS_ID");
    int gpNo = query.record().indexOf("GOODS_PRICE");
    orders.reserve(query.record().count());
    while (query.next())
    {
        Order* order = new Order();
        order->id = query.value(idNo).toString();
        order->title = query.value(titleNo).toString();
        order->price = query.value(priceNo).toString();
        order->count = query.value(countNo).toString();
        order->user_remark = query.value(urNo).toString();
        order->sell_remark = query.value(srNo).toString();
        order->user_name = query.value(unameNo).toString();
        order->goods_id = query.value(gidNo).toString();
        order->goods_price = query.value(gpNo).toString();
        orders.append(order);
    }
}

bool OrderStore::attachGoods(Order* order)
{
    if (NULL == order || order->goods_id.isEmpty())
    {
        return false;
    }

    if (NULL == m_db || NULL == order)
    {
        return false;
    }

    QString updateSql = QString(kUpdateSettledSQL).arg(order->date);
    QSqlQuery query;
    query.prepare(updateSql);
    query.addBindValue(order->goods_id);
    query.addBindValue(order->goods_price);
    query.addBindValue(order->id);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool OrderStore::detachGoods(Order* order)
{
    if (NULL == order)
    {
        return false;
    }

    QString updateSql = QString(kUpdateSettledSQL).arg(order->date);
    QSqlQuery query;
    query.prepare(updateSql);
    query.addBindValue("");
    query.addBindValue("");
    query.addBindValue(order->id);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool OrderStore::update()
{
    return false;
}
