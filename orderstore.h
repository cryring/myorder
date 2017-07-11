#ifndef ORDERSTORE_H
#define ORDERSTORE_H

#include <QSqlDatabase>
#include "orderdefine.h"

class QSqlDatabase;

class OrderStore
{
public:
    OrderStore();

    ~OrderStore();

public:
    bool init(QSqlDatabase* db);

    bool insert(const QString& date, Order* order);

    bool update();

private:
    bool createTable();

private:
    QSqlDatabase* m_db;
};

#endif // ORDERSTORE_H
