#ifndef GOODSNAMESTORE_H
#define GOODSNAMESTORE_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QSet>
#include "singleton.h"

class QSqlDatabase;

struct GoodsName
{
    QString id;
    QString brand;
    QString name;
};

typedef QMap<QString, GoodsName*> GNMAP;

class GoodsNameStore : public Singleton<GoodsNameStore>
{
public:
    GoodsNameStore();

    ~GoodsNameStore();

public:
    bool init(QSqlDatabase* db);

    bool insert(const QString& id, const QString& brand, const QString& name);

    bool remove(const QString& id);

    const GNMAP& getNames(void);

    GoodsName* get(const QString& id);

private:
    GNMAP m_names;
};

#endif // GOODSNAMESTORE_H
