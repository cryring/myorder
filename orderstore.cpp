#include <QDebug>
#include <QtSql>
#include "orderstore.h"

static const char* kTestSQL = "CREATE TABLE IF NOT EXISTS TEST(" \
                              "ID VARCHAR PRIMARY KEY NOT NULL," \
                              "NAME VARCHAR NOT NULL," \
                              "DATE VARCHAR NOT NULL)";

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS ORDER_%1(" \
                                "ID VARCHAR PRIMARY KEY NOT NULL," \
                                "TITLE VARCHAR NOT NULL," \
                                "PRICE VARCHAR NOT NULL," \
                                "COUNT VARCHAR NOT NULL," \
                                "USER_REMARK VARCHAR," \
                                "SELL_REMARK VARCHAR," \
                                "USER_NAME VARCHAR NOT NULL)";

static const char* kInsertSQL = "INSERT INTO ORDER_%1(ID,TITLE,PRICE,COUNT,USER_REMARK,SELL_REMARK,USER_NAME) VALUES(?,?,?,?,?,?,?)";

static const char* kSelectByDateSQL = "SELECT * FROM ORDER_%1";

OrderStore::OrderStore()
:m_db(NULL)
{

}

OrderStore::~OrderStore()
{

}

bool OrderStore::init(QSqlDatabase* db)
{
    m_db = db;
    QSqlQuery query;
    query.prepare(kTestSQL);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

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
        orders.append(order);
    }
}

bool OrderStore::update()
{
    return false;
}
