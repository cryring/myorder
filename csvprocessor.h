#ifndef CSVPROCESSOR_H
#define CSVPROCESSOR_H

#include <QStringList>

class CsvProcessor
{
public:
    virtual bool process(const QStringList& fields) = 0;
};

#endif // CSVPROCESSOR_H
