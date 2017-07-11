#include <QtSql>
#include <QtDebug>
#include "goodsstore.h"

static const char* kTestSQL = "CREATE TABLE IF NOT EXISTS TEST(" \
                              "ID VARCHAR PRIMARY KEY NOT NULL," \
                              "NAME VARCHAR NOT NULL," \
                              "DATE VARCHAR NOT NULL)";

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS GOODS_%1(" \
                                  "INVOICEID VARCHAR NOT NULL," \
                                  "SHOPNAME VARCHAR NOT NULL," \
                                  "NAME VARCHAR NOT NULL," \
                                  "PRICE VARCHAR NOT NULL," \
                                  "ATTRIBUTE VARCHAR NOT NULL," \
                                  "COUNT VARCHAR NOT NULL," \
                                  "CURRENCY VARCHAR NOT NULL," \
                                  "PAYMENTMETHOD VARCHAR NOT NULL," \
                                  "EXCHANGERATE VARCHAR NOT NULL," \
                                  "DISCOUNT VARCHAR NOT NULL," \
                                  "REBATE VARCHAR NOT NULL)";

static const char* kInsertSQL = "INSERT INTO GOODS_%1(INVOICEID,SHOPNAME,NAME,PRICE,ATTRIBUTE,COUNT,CURRENCY,PAYMENTMETHOD,EXCHANGERATE,DISCOUNT,REBATE) VALUES(?,?,?,?,?,?,?,?,?,?,?)";

GoodsStore::GoodsStore()
:m_db(NULL)
{

}

GoodsStore::~GoodsStore()
{

}

bool GoodsStore::init(QSqlDatabase* db)
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

bool GoodsStore::insert(const QString &date, Goods *goods)
{
    if (NULL == m_db || NULL == goods)
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
    query.addBindValue(goods->invoiceid);
    query.addBindValue(goods->shopName);
    query.addBindValue(goods->name);
    query.addBindValue(goods->price);
    query.addBindValue(goods->attribute);
    query.addBindValue(goods->count);
    query.addBindValue(goods->currency);
    query.addBindValue(goods->paymentMethod);
    query.addBindValue(goods->exchangeRate);
    query.addBindValue(goods->discount);
    query.addBindValue(goods->rebate);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}
