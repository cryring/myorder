#ifndef INVOICES_H
#define INVOICES_H

#include "singleton.h"

class Invoices : public Singleton<Invoices>
{
public:
    Invoices();
};

#endif // INVOICES_H
