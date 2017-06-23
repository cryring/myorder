#ifndef ORDERSTORE_H
#define ORDERSTORE_H

#include <QSqlDatabase>
#include "orderdefine.h"

class OrderStore
{
public:
    OrderStore();

    ~OrderStore();

public:
    bool init();

    bool insert(OrderDetail* order);

    bool update();

private:
    bool createTable();
};

#endif // ORDERSTORE_H
