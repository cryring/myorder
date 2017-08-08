#include <QtSql>
#include <QtDebug>
#include <QUuid>
#include "goodsstore.h"

static const char* kTestSQL = "CREATE TABLE IF NOT EXISTS TEST(" \
                              "ID VARCHAR PRIMARY KEY NOT NULL," \
                              "NAME VARCHAR NOT NULL," \
                              "DATE VARCHAR NOT NULL)";

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS GOODS_%1(" \
                                "ID VARCHAR PRIMARY KEY NOT NULL," \
                                "INVOICEID VARCHAR NOT NULL," \
                                "SHOPNAME VARCHAR NOT NULL," \
                                "NAME VARCHAR NOT NULL," \
                                "PRICE VARCHAR NOT NULL," \
                                "ATTRIBUTE VARCHAR NOT NULL," \
                                "COUNT VARCHAR NOT NULL," \
                                "SETTLED INT NOT NULL)";

static const char* kInsertSQL = "INSERT INTO GOODS_%1(ID,INVOICEID,SHOPNAME,NAME,PRICE,ATTRIBUTE,COUNT,SETTLED) VALUES(?,?,?,?,?,?,?,?)";
static const char* kUpdateSettled = "UPDATE GOODS_%1 SET SETTLED=? WHERE ID=?";

GoodsStore::GoodsStore()
    : m_db(NULL)
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

bool GoodsStore::insert(const QString& date, Goods* goods)
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
    query.addBindValue(QUuid::createUuid().toString());
    query.addBindValue(goods->invoiceId);
    query.addBindValue(goods->shopName);
    query.addBindValue(goods->name);
    query.addBindValue(goods->price);
    query.addBindValue(goods->attribute);
    query.addBindValue(goods->count);
    query.addBindValue(0);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool GoodsStore::updateSettle(Goods* goods, bool settled)
{
    if (NULL == m_db || NULL == goods)
    {
        return false;
    }

    QString updateSql = QString(kUpdateSettled).arg(goods->date);
    QSqlQuery query;
    query.prepare(updateSql);
    query.addBindValue(settled ? 1 : 0);
    query.addBindValue(goods->id);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}
