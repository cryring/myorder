#include <QFile>
#include <QTextStream>
#include "csvloader.h"
#include "utils.h"

CsvLoader::CsvLoader(CsvProcessor* processor)
:m_processor(processor)
{

}

bool CsvLoader::Load(const QString &filename)
{
    QFile file(filename);
    if (false == file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }


    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull())
    {
        if (NULL != m_processor)
        {
            QStringList list = line.split(",");
            m_processor->process(list);
        }
        line = in.readLine();
    }

    file.close();
    return true;
}
