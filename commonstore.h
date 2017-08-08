#ifndef COMMONSTORE_H
#define COMMONSTORE_H

#include <QVector>
#include <QString>
#include "singleton.h"

class QSqlDatabase;
class QSqlQuery;

class CommonStore : public Singleton<CommonStore>
{
public:
    CommonStore(void);

    ~CommonStore(void);

public:
    bool init(QSqlDatabase* db);

    QString get(const QString& key);

private:
    QSqlDatabase *m_db;

};

#endif // COMMONSTORE_H
