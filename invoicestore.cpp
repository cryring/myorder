#include <QDebug>
#include <QtSql>
#include "invoicestore.h"

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS INVOICE_%1(" \
                                "ID VARCHAR PRIMARY KEY NOT NULL," \
                                "DATE VARCHAR NOT NULL," \
                                "CASH VARCHAR," \
                                "CASHRATE VARCHAR," \
                                "CASHCHANGE VARCHAR," \
                                "COUPON VARCHAR," \
                                "COUPONDISCOUNT VARCHAR," \
                                "COUPONRATE VARCHAR," \
                                "COUPONCHANGE VARCHAR," \
                                "CREDITCARD VARCHAR)";

static const char* kInsertSQL = "INSERT INTO INVOICE_%1(ID,DATE,CASH,CASHRATE,CASHCHANGE,COUPON,COUPONDISCOUNT,COUPONRATE,COUPONCHANGE,CREDITCARD) VALUES(?,?,?,?,?,?,?,?,?,?)";

static const char* kSelectSQL = "SELECT * FROM INVOICE_%1";

static const char* kDeleteSQL = "DELETE FROM INVOICE_%1 WHERE ID=?";


InvoiceStore::InvoiceStore()
{

}

bool InvoiceStore::init(void)
{
    return true;
}

bool InvoiceStore::set(InvoiceBase* ib)
{
    if (NULL == ib)
    {
        return false;
    }

    QString sql = QString(kCreateSQL).arg(ib->date.left(6));
    QSqlQuery query;
    query.prepare(sql);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    QString date;
    QString id;
    QString cash;
    QString cashRate;
    QString cashChange;
    QString coupon;
    QString couponDiscount;
    QString couponRate;
    QString couponChange;
    QString creditCard;

    sql = QString(kInsertSQL).arg(ib->date.left(6));
    query.prepare(sql);
    query.addBindValue(ib->id);
    query.addBindValue(ib->date);
    query.addBindValue(ib->cash);
    query.addBindValue(ib->cashRate);
    query.addBindValue(ib->cashChange);
    query.addBindValue(ib->coupon);
    query.addBindValue(ib->couponDiscount);
    query.addBindValue(ib->couponRate);
    query.addBindValue(ib->couponChange);
    query.addBindValue(ib->creditCard);
    if (false == query.exec())
    {
        qDebug() << "insert" << ib->id << "error:" << query.lastError();
        return false;
    }

    return true;
}

InvoiceBase* InvoiceStore::get(const QString& id)
{
    return NULL;
}

bool InvoiceStore::del(const QString& id)
{
    return true;
}
