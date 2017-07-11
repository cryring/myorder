#include <QDebug>
#include <QtSql>
#include "orderstore.h"

static const char* kTestSQL = "CREATE TABLE IF NOT EXISTS TEST(" \
                              "ID VARCHAR PRIMARY KEY NOT NULL," \
                              "NAME VARCHAR NOT NULL," \
                              "DATE VARCHAR NOT NULL)";

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS ORDER_%1(" \
                                "ID VARCHAR PRIMARY KEY NOT NULL," \
                                "NAME VARCHAR NOT NULL," \
                                "PRICE VARCHAR NOT NULL," \
                                "USER_REMARK VARCHAR," \
                                "SELL_REMARK VARCHAR," \
                                "USER_NAME VARCHAR NOT NULL)";

static const char* kInsertSQL = "INSERT INTO ORDER_%1(ID,NAME,PRICE,USER_REMARK,SELL_REMARK,USER_NAME) VALUES(?,?,?,?,?,?)";

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

bool OrderStore::update()
{
    return false;
}
