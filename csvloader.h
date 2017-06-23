#ifndef CSVLOADER_H
#define CSVLOADER_H

#include <QString>
#include "csvprocessor.h"

class CsvLoader
{
public:
    CsvLoader(CsvProcessor* processor);

public:
    bool Load(const QString& filename);

private:
    CsvProcessor* m_processor;
};

#endif // CSVLOADER_H
