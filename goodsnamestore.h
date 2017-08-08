#ifndef GOODSNAMESTORE_H
#define GOODSNAMESTORE_H

#include <QString>
#include <QVector>
#include <QMap>
#include "singleton.h"

class QSqlDatabase;

typedef QMap<QString, QVector<QString> > GNMAP;

class GoodsNameStore : public Singleton<GoodsNameStore>
{
public:
    GoodsNameStore();

public:
    bool init(QSqlDatabase* db);

    bool insert(const QString& brand, const QString& name);

    const GNMAP& getNames(void);

private:
    GNMAP m_names;
};

#endif // GOODSNAMESTORE_H
