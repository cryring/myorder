#ifndef DBOPERATION_H
#define DBOPERATION_H

#include <QStringList>

class DBOperation
{
public:
    DBOperation();

public:
    static QStringList getAllTableName(void);
};

#endif // DBOPERATION_H
