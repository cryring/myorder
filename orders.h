#ifndef ORDERS_H
#define ORDERS_H

#include "singleton.h"

class Orders : public Singleton<Orders>
{
public:
    Orders();
};

#endif // ORDERS_H
