#ifndef PROFITSTORE_H
#define PROFITSTORE_H

#include <QString>
#include "singleton.h"

class ProfitStore : public Singleton<ProfitStore>
{
public:
    ProfitStore();

public:
    bool insert(const QString& date, const QString& profit);
    bool update(const QString& date, const QString& profit);
};

#endif // PROFITSTORE_H
