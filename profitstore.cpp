#include <QDebug>
#include <QtSql>
#include "profitstore.h"

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS PROFIT(" \
                                "ID VARCHAR PRIMARY KEY NOT NULL," \
                                "PROFIT VARCHAR)";

static const char* kInsertSQL = "INSERT INTO PROFIT(ID,PROFIT) VALUES(?,?)";

static const char* kSelectSQL = "SELECT * FROM PROFIT";

static const char* kUpdateSQL = "UPDATE PROFIT SET PROFIT=? WHERE ID=?";


ProfitStore::ProfitStore()
{

}

bool ProfitStore::insert(const QString& date, const QString& profit)
{
    if (date.isEmpty())
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

    QString insertSql = kInsertSQL;
    query.prepare(insertSql);
    query.addBindValue(date);
    query.addBindValue(profit);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

bool ProfitStore::update(const QString& date, const QString& profit)
{
    if (date.isEmpty())
    {
        return false;
    }
    QString updateSql = kUpdateSQL;
    QSqlQuery query;
    query.prepare(updateSql);
    query.addBindValue(profit);
    query.addBindValue(date);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}
