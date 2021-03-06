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

    bool insert(Order* order);

    void select(const QString& date, QVector<Order*>& orders);

    bool attachGoods(Order* order);

    bool detachGoods(Order* order);

    bool update();

private:
    bool createTable();

private:
    QSqlDatabase* m_db;
};

#endif // ORDERSTORE_H
