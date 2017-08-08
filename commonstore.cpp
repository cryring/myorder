#include <QtSql>
#include "commonstore.h"
#include "goodsnamestore.h"
#include "shopnamestore.h"

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS COMMON(" \
                                "KEY VARCHAR PRIMARY KEY NOT NULL," \
                                "VAL VARCHAR NOT NULL)";

static const char* kGetAllSQL = "SELECT * FROM COMMON";

CommonStore::CommonStore(void)
{

}

CommonStore::~CommonStore()
{

}

bool CommonStore::init(QSqlDatabase *db)
{
    m_db = db;
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
    else
    {
        // importShopNames(&query);
    }

    return true;
}

QString CommonStore::get(const QString& key)
{
    return "";
}




