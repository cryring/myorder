#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>
#include "csvloader.h"
#include "utils.h"

CsvLoader::CsvLoader(CsvProcessor* processor)
    : m_processor(processor)
{

}

inline QString GBK2UTF8(const QString &in)
{
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QString g2u = utf8->toUnicode(in.toUtf8());
    return g2u;
}

bool CsvLoader::Load(const QString& filename)
{
    QFile file(filename);
    if (false == file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(&file);
    in.setCodec("GB18030");
    QString all = in.readAll();

    QString uall = GBK2UTF8(all);
    QTextStream uin(&all);

    QString line = uin.readLine();
    while (!line.isNull())
    {
        if (NULL != m_processor)
        {
            QStringList list = line.split(",");
            m_processor->process(list);
        }
        line = uin.readLine();
    }

    file.close();
    return true;
}
