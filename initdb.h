#ifndef INITDB_H
#define INITDB_H

#include <QtSql>

const char* createOrderTable = "";
const char* createInvoiceTable = "";

QSqlError initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/User/siwen/code/test.db");

    if (!db.open())
    {
        return db.lastError();
    }

    QStringList tables = db.tables();
    if (tables.contains("orders", Qt::CaseInsensitive) &&
        tables.contains("invoice", Qt::CaseInsensitive))
    {
        return QSqlError();
    }

    QSqlQuery q;
    if (!q.exec(QLatin1String(createOrderTable)))
    {
        return q.lastError();
    }
    if (!q.exec(QLatin1String(createInvoiceTable)))
    {
        return q.lastError();
    }

    return QSqlError();
}

#endif // INITDB_H
