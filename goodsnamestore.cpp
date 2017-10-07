#include <QtSql>
#include "goodsnamestore.h"

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS GOODSNAMES(" \
                                "ID VARCHAR PRIMARY KEY," \
                                "BRAND VARCHAR NOT NULL," \
                                "NAME VARCHAR NOT NULL)";

static const char* kGetAllSQL = "SELECT * from GOODSNAMES";

static const char* kInsertSQL = "INSERT INTO GOODSNAMES(ID,BRAND,NAME) VALUES(?,?,?)";

static const char* kDeleteSQL = "DELETE FROM GOODSNAMES WHERE ID=?";

GoodsNameStore::GoodsNameStore()
{

}

GoodsNameStore::~GoodsNameStore()
{

}

bool GoodsNameStore::init(QSqlDatabase* db)
{
    QSqlQuery query;
    query.prepare(kCreateSQL);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    query.prepare(kGetAllSQL);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    int idNo = query.record().indexOf("ID");
    int brandNo = query.record().indexOf("BRAND");
    int nameNo = query.record().indexOf("NAME");
    while (query.next())
    {
        GoodsName* gn = new GoodsName();
        gn->id = query.value(idNo).toString();
        gn->brand = query.value(brandNo).toString();
        gn->name = query.value(nameNo).toString();
        m_names.insert(gn->id, gn);
    }

    return true;
}

bool GoodsNameStore::insert(const QString& id, const QString& brand, const QString& name)
{
    if (m_names.contains(id))
    {
        return false;
    }

    QSqlQuery query;
    query.prepare(kInsertSQL);
    query.addBindValue(id);
    query.addBindValue(brand);
    query.addBindValue(name);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    GoodsName* gn = new GoodsName();
    gn->id = id;
    gn->brand = brand;
    gn->name = name;
    m_names.insert(id, gn);

    return true;
}

bool GoodsNameStore::remove(const QString& id)
{
    QSqlQuery query;
    query.prepare(kDeleteSQL);
    query.addBindValue(id);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    m_names.remove(id);

    return true;
}

const GNMAP& GoodsNameStore::getNames()
{
    return m_names;
}

GoodsName* GoodsNameStore::get(const QString& id)
{
    if (m_names.contains(id))
    {
        return m_names[id];
    }
    return NULL;
}
