#include <QDebug>
#include <QtSql>
#include "shopnamestore.h"

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS SHOPNAMES(" \
                                "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                                "NAME VARCHAR NOT NULL)";

static const char* kGetAllSQL = "SELECT NAME from SHOPNAMES";

static const char* kInsertSQL = "INSERT INTO SHOPNAMES(NAME) VALUES(?)";

ShopNameStore::ShopNameStore()
{

}

bool ShopNameStore::init(QSqlDatabase* db)
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

    int nameNo = query.record().indexOf("NAME");
    m_names.reserve(query.record().count());
    while (query.next())
    {
        QString name = query.value(nameNo).toString();
        m_names.append(name);
    }

    return true;
}

bool ShopNameStore::insert(const QString& name)
{
    QSqlQuery query;
    query.prepare(kInsertSQL);
    query.addBindValue(name);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    m_names.append(name);

    return true;
}

const QVector<QString>& ShopNameStore::getNames()
{
    return m_names;
}
