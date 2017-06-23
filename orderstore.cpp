#include <QDebug>
#include "orderstore.h"

OrderStore::OrderStore()
{
    qDebug() << QSqlDatabase::drivers();
}

OrderStore::~OrderStore()
{

}

bool OrderStore::init()
{
    return false;
}

bool OrderStore::insert(OrderDetail* order)
{
    if (NULL == order)
    {
        return false;
    }

    return false;
}

bool OrderStore::update()
{
    return false;
}
