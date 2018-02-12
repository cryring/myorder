#include <QDebug>
#include <QtSql>
#include "invoicestore.h"
#include "dboperation.h"

static const char* kCreateSQL = "CREATE TABLE IF NOT EXISTS INVOICE_%1(" \
                                "ID VARCHAR PRIMARY KEY NOT NULL," \
                                "DATE VARCHAR NOT NULL," \
                                "SHOPNAME VARCHAR NOT NULL," \
                                "CASH VARCHAR," \
                                "CASHRATE VARCHAR," \
                                "CASHCHANGE VARCHAR," \
                                "COUPON VARCHAR," \
                                "COUPONDISCOUNT VARCHAR," \
                                "COUPONRATE VARCHAR," \
                                "CREDITCARD VARCHAR)";

static const char* kInsertSQL = "INSERT INTO INVOICE_%1(ID,DATE,SHOPNAME,CASH,CASHRATE,CASHCHANGE,COUPON,COUPONDISCOUNT,COUPONRATE,CREDITCARD) VALUES(?,?,?,?,?,?,?,?,?,?)";

static const char* kSelectSQL = "SELECT * FROM INVOICE_%1 WHERE ID=?";

static const char* kSelectByDateSQL = "SELECT * FROM INVOICE_%1";

static const char* kDeleteSQL = "DELETE FROM INVOICE_%1 WHERE ID=?";


InvoiceStore::InvoiceStore()
{

}

InvoiceStore::~InvoiceStore()
{
    QMap<QString, InvoiceBase*>::iterator it = m_ibs.begin();
    for (; it != m_ibs.end(); it++)
    {
        delete it.value();
    }
    m_ibs.clear();
}

bool InvoiceStore::init(void)
{
    QStringList tables = DBOperation::getAllTableName();
    for (int i = 0; i < tables.size(); ++i)
    {
        QString tableName = tables[i];
        if (0 == tableName.indexOf("INVOICE_"))
        {
            QString date = tableName.replace("INVOICE_", "");
            QVector<InvoiceBase*> ibs;
            getByDate(date, ibs);
            for (int k = 0; k < ibs.size(); k++)
            {
                m_ibs.insert(ibs[k]->id, ibs[k]);
            }
        }
    }
    return true;
}

bool InvoiceStore::set(InvoiceBase* ib)
{
    if (NULL == ib)
    {
        return false;
    }

    QString sql = QString(kCreateSQL).arg(ib->date.left(8));
    QSqlQuery query;
    query.prepare(sql);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }

    sql = QString(kInsertSQL).arg(ib->date.left(8));
    query.prepare(sql);
    query.addBindValue(ib->id);
    query.addBindValue(ib->date);
    query.addBindValue(ib->shopName);
    query.addBindValue(ib->cash);
    query.addBindValue(ib->cashRate);
    query.addBindValue(ib->cashChange);
    query.addBindValue(ib->coupon);
    query.addBindValue(ib->couponDiscount);
    query.addBindValue(ib->couponRate);
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
    QString date = id.left(8);
    QString sql = QString(kSelectSQL).arg(date);
    qDebug() << sql << id;
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
        int idNo = query.record().indexOf("ID");
        int dateNo = query.record().indexOf("DATE");
        int shopNameNo = query.record().indexOf("SHOPNAME");
        int cashNo = query.record().indexOf("CASH");
        int cashRateNo = query.record().indexOf("CASHRATE");
        int cashChangeNo = query.record().indexOf("CASHCHANGE");
        int couponNo = query.record().indexOf("COUPON");
        int couponDiscountNo = query.record().indexOf("COUPONDISCOUNT");
        int couponRateNo = query.record().indexOf("COUPONRATE");
        int creditCardNo = query.record().indexOf("CREDITCARD");


        InvoiceBase* ib = new InvoiceBase();
        ib->id = query.value(idNo).toString();
        ib->date = query.value(dateNo).toString();
        ib->shopName = query.value(shopNameNo).toString();
        ib->cash = query.value(cashNo).toString();
        ib->cashRate = query.value(cashRateNo).toString();
        ib->cashChange = query.value(cashChangeNo).toString();
        ib->coupon = query.value(couponNo).toString();
        ib->couponDiscount = query.value(couponDiscountNo).toString();
        ib->couponRate = query.value(couponRateNo).toString();
        ib->creditCard = query.value(creditCardNo).toString();

        return ib;
    }
    return NULL;
}

void InvoiceStore::getByDate(const QString& date, QVector<InvoiceBase*>& ibs)
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
    int dateNo = query.record().indexOf("DATE");
    int shopNameNo = query.record().indexOf("SHOPNAME");
    int cashNo = query.record().indexOf("CASH");
    int cashRateNo = query.record().indexOf("CASHRATE");
    int cashChangeNo = query.record().indexOf("CASHCHANGE");
    int couponNo = query.record().indexOf("COUPON");
    int couponDiscountNo = query.record().indexOf("COUPONDISCOUNT");
    int couponRateNo = query.record().indexOf("COUPONRATE");
    int creditCardNo = query.record().indexOf("CREDITCARD");

    while (query.next())
    {
        InvoiceBase* ib = new InvoiceBase();
        ib->id = query.value(idNo).toString();
        ib->date = query.value(dateNo).toString();
        ib->shopName = query.value(shopNameNo).toString();
        ib->cash = query.value(cashNo).toString();
        ib->cashRate = query.value(cashRateNo).toString();
        ib->cashChange = query.value(cashChangeNo).toString();
        ib->coupon = query.value(couponNo).toString();
        ib->couponDiscount = query.value(couponDiscountNo).toString();
        ib->couponRate = query.value(couponRateNo).toString();
        ib->creditCard = query.value(creditCardNo).toString();
        ibs.append(ib);
    }
}

bool InvoiceStore::del(const QString& id)
{
    return true;
}
