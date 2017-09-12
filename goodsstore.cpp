#include <QtSql>
#include <QtDebug>
#include <QUuid>
#include "goodsstore.h"

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS GOODS_%1(" \
                                "ID VARCHAR PRIMARY KEY NOT NULL," \
                                "INVOICEID VARCHAR NOT NULL," \
                                "SHOPNAME VARCHAR NOT NULL," \
                                "NAME VARCHAR NOT NULL," \
                                "PRICE VARCHAR NOT NULL," \
                                "ATTRIBUTE VARCHAR NOT NULL," \
                                "COUNT VARCHAR NOT NULL," \
                                "SETTLED INT NOT NULL)";

static const char* kSelectByDateSQL = "SELECT * FROM GOODS_%1";
static const char* kSelectByID = "SELECT * FROM GOODS_%1 WHERE ID=?";
static const char* kInsertSQL = "INSERT INTO GOODS_%1(ID,INVOICEID,SHOPNAME,NAME,PRICE,ATTRIBUTE,COUNT,SETTLED) VALUES(?,?,?,?,?,?,?,?)";
static const char* kUpdateSQL = "UPDATE GOODS_%1 SET SETTLED=? WHERE ID=?";

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
    query.addBindValue(goods->id);
    query.addBindValue(goods->invoiceid);
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

void GoodsStore::select(const QString& date, QVector<Goods*>& goodss)
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
    int iidNo = query.record().indexOf("INVOICEID");
    int shopnameNo = query.record().indexOf("SHOPNAME");
    int nameNo = query.record().indexOf("NAME");
    int priceNo = query.record().indexOf("PRICE");
    int attrNo = query.record().indexOf("ATTRIBUTE");
    int cntNo = query.record().indexOf("COUNT");
    int settledNo = query.record().indexOf("SETTLED");
    goodss.reserve(query.record().count());
    while (query.next())
    {
        Goods* goods = new Goods();
        goods->id = query.value(idNo).toString();
        goods->invoiceid = query.value(iidNo).toString();
        goods->shopName = query.value(shopnameNo).toString();
        goods->name = query.value(nameNo).toString();
        goods->price = query.value(priceNo).toString();
        goods->attribute = query.value(attrNo).toString();
        goods->count = query.value(cntNo).toString();
        goods->settled = query.value(settledNo).toInt() ? true : false;
        goodss.append(goods);
    }
}

Goods* GoodsStore::selectGoodsByID(const QString &goods_id)
{
    auto date = goods_id.left(8);
    QString sql = QString(kSelectByID).arg(date);
    QSqlQuery query;
    query.prepare(sql);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return NULL;
    }

    int idNo = query.record().indexOf("ID");
    int shopnameNo = query.record().indexOf("SHOPNAME");
    int nameNo = query.record().indexOf("NAME");
    int priceNo = query.record().indexOf("PRICE");
    int attrNo = query.record().indexOf("ATTRIBUTE");
    int cntNo = query.record().indexOf("COUNT");
    int settledNo = query.record().indexOf("SETTLED");
    if (query.next())
    {
        Goods* goods = new Goods();
        goods->id = query.value(idNo).toString();
        goods->shopName = query.value(shopnameNo).toString();
        goods->name = query.value(nameNo).toString();
        goods->price = query.value(priceNo).toString();
        goods->attribute = query.value(attrNo).toString();
        goods->count = query.value(cntNo).toString();
        goods->settled = query.value(settledNo).toInt() ? true : false;
        return goods;
    }
    return NULL;
}

bool GoodsStore::updateSettle(const QString& goods_id, bool settled)
{
    if (NULL == m_db || goods_id.isEmpty())
    {
        return false;
    }

    auto date = goods_id.left(8);
    QString updateSql = QString(kUpdateSQL).arg(date);
    QSqlQuery query;
    query.prepare(updateSql);
    query.addBindValue(settled ? 1 : 0);
    query.addBindValue(goods_id);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}
