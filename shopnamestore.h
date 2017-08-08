#ifndef SHOPNAMESTORE_H
#define SHOPNAMESTORE_H

#include <QString>
#include <QVector>
#include "singleton.h"

class QSqlDatabase;

class ShopNameStore : public Singleton<ShopNameStore>
{
public:
    ShopNameStore();

public:
    bool init(QSqlDatabase* db);

    bool insert(const QString& name);

    const QVector<QString>& getNames(void);

private:
    QVector<QString> m_names;
};

#endif // SHOPNAMESTORE_H
