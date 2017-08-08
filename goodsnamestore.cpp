#include <QtSql>
#include "goodsnamestore.h"

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS GOODSNAMES(" \
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                "BRAND VARCHAR NOT NULL," \
                                "NAME VARCHAR NOT NULL)";

static const char* kGetAllSQL = "SELECT * from GOODSNAMES";

static const char* kInsertSQL = "INSERT INTO GOODSNAMES(BRAND,NAME) VALUES(?,?)";

GoodsNameStore::GoodsNameStore()
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

    int brandNo = query.record().indexOf("BRAND");
    int nameNo = query.record().indexOf("NAME");
    while (query.next())
    {
        QString brand = query.value(brandNo).toString();
        QString name = query.value(nameNo).toString();
        m_names[brand].append(name);
    }

    return true;
}

bool GoodsNameStore::insert(const QString& brand, const QString& name)
{
    QSqlQuery query;
    query.prepare(kInsertSQL);
    query.addBindValue(brand);
    query.addBindValue(name);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    m_names[brand].append(name);

    return true;
}

const GNMAP& GoodsNameStore::getNames()
{
    return m_names;
}
