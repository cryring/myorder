#include <QtDebug>
#include <QtSql>
#include "dboperation.h"

DBOperation::DBOperation()
{

}

QStringList DBOperation::getAllTableName(void)
{
    QString sql = QString("SELECT name FROM sqlite_master WHERE type='table'");
    QSqlQuery query;
    query.prepare(sql);
    if (false == query.exec())
    {
        qDebug() << query.lastError();
        return QStringList();
    }

    QStringList names;
    int nameNo = query.record().indexOf("name");
    while (query.next())
    {
        names.append(query.value(nameNo).toString());
    }
    return names;
}
