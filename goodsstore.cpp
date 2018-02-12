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
                                "REALPRICE VARCHAR NOT NULL," \
                                "ATTRIBUTE VARCHAR NOT NULL," \
                                "COUNT VARCHAR NOT NULL," \
                                "SETTLED INT NOT NULL)";

static const char* kSelectByDateSQL = "SELECT * FROM GOODS_%1";
static const char* kSelectByID = "SELECT * FROM GOODS_%1 WHERE ID=?";
static const char* kSelectByInvoiceID = "SELECT * FROM GOODS_%1 WHERE INVOICEID=?";
static const char* kInsertSQL = "INSERT INTO GOODS_%1(ID,INVOICEID,SHOPNAME,NAME,PRICE,REALPRICE,ATTRIBUTE,COUNT,SETTLED) VALUES(?,?,?,?,?,?,?,?,?)";
static const char* kUpdateSQL = "UPDATE GOODS_%1 SET SHOPNAME=?,NAME=?,PRICE=?,ATTRIBUTE=?,COUNT=? WHERE ID=?";
static const char* kUpdateSettledSQL = "UPDATE GOODS_%1 SET SETTLED=? WHERE ID=?";
static const char* kDeleteSQL = "DELETE FROM GOODS_%1 WHERE ID=?";
static const char* kDeleteInvoiceSQL = "DELETE FROM GOODS_%1 WHERE INVOICEID=?";

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

bool GoodsStore::insert(Goods* goods)
{
    if (NULL == m_db || NULL == goods)
    {
        return false;
    }

    QString createSql = QString(kCreateSQL).arg(goods->date);
    QSqlQuery query;
    query.prepare(createSql);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    QString insertSql = QString(kInsertSQL).arg(goods->date);
    query.prepare(insertSql);
    query.addBindValue(goods->id);
    query.addBindValue(goods->invoiceid);
    query.addBindValue(goods->shopName);
    query.addBindValue(goods->name);
    query.addBindValue(goods->price);
    query.addBindValue(goods->realPrice);
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
    int realPriceNo = query.record().indexOf("REALPRICE");
    int attrNo = query.record().indexOf("ATTRIBUTE");
    int cntNo = query.record().indexOf("COUNT");
    int settledNo = query.record().indexOf("SETTLED");
    goodss.reserve(query.record().count());
    while (query.next())
    {
        Goods* goods = new Goods();
        goods->id = query.value(idNo).toString();
        goods->date = date;
        goods->invoiceid = query.value(iidNo).toString();
        goods->shopName = query.value(shopnameNo).toString();
        goods->name = query.value(nameNo).toString();
        goods->price = query.value(priceNo).toString();
        goods->realPrice = query.value(realPriceNo).toString();
        goods->attribute = query.value(attrNo).toString();
        goods->count = query.value(cntNo).toString();
        goods->settled = query.value(settledNo).toInt() ? true : false;
        goodss.append(goods);
    }
}

void GoodsStore::selectGoodsByInvoice(const QString& invoiceid, QVector<Goods*>& goodss)
{
    QString date = invoiceid.left(8);
    QString sql = QString(kSelectByInvoiceID).arg(date);
    QSqlQuery query;
    query.prepare(sql);
    query.addBindValue(invoiceid);
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
    int realPriceNo = query.record().indexOf("REALPRICE");
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
        goods->realPrice = query.value(realPriceNo).toString();
        goods->attribute = query.value(attrNo).toString();
        goods->count = query.value(cntNo).toString();
        goods->settled = query.value(settledNo).toInt() ? true : false;
        goodss.append(goods);
    }
}

bool GoodsStore::update(Goods* goods)
{
    if (NULL == m_db || NULL == goods)
    {
        return false;
    }

    QString sql = QString(kUpdateSQL).arg(goods->date);
    QSqlQuery query;
    query.prepare(sql);
    query.addBindValue(goods->shopName);
    query.addBindValue(goods->name);
    query.addBindValue(goods->price);
    query.addBindValue(goods->attribute);
    query.addBindValue(goods->count);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool GoodsStore::remove(Goods* goods)
{
    if (NULL == m_db)
    {
        return false;
    }

    QString removeSql = QString(kDeleteSQL).arg(goods->date);
    QSqlQuery query;
    query.prepare(removeSql);
    query.addBindValue(goods->id);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool GoodsStore::removeByInvoiceID(const QString& date, const QString& invoiceid)
{
    if (NULL == m_db)
    {
        return false;
    }

    QString removeSql = QString(kDeleteInvoiceSQL).arg(date);
    QSqlQuery query;
    query.prepare(removeSql);
    query.addBindValue(invoiceid);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool GoodsStore::exist(const QString& id)
{
    auto date = id.left(8);
    QString sql = QString(kSelectByID).arg(date);
    QSqlQuery query;
    query.prepare(sql);
    query.addBindValue(id);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return NULL;
    }

    if (query.next())
    {
        return true;
    }
    return false;
}

Goods* GoodsStore::selectGoodsByID(const QString &id)
{
    auto date = id.left(8);
    QString sql = QString(kSelectByID).arg(date);
    QSqlQuery query;
    query.prepare(sql);
    query.addBindValue(id);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return NULL;
    }

    int idNo = query.record().indexOf("ID");
    int iidNo  = query.record().indexOf("INVOICEID");
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
        goods->invoiceid = query.value(iidNo).toString();
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

bool GoodsStore::updateSettle(const QString& id, bool settled)
{
    if (NULL == m_db || id.isEmpty())
    {
        return false;
    }

    auto date = id.left(8);
    QString updateSql = QString(kUpdateSettledSQL).arg(date);
    QSqlQuery query;
    query.prepare(updateSql);
    query.addBindValue(settled ? 1 : 0);
    query.addBindValue(id);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}
