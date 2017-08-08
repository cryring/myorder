#include <QApplication>
#include <QTextCodec>
#include <QtSql>
#include <QtDebug>
#include "dialog.h"
#include "commonstore.h"
#include "goodsnamestore.h"
#include "shopnamestore.h"
#include "store.h"

class DBDefer
{
public:
    DBDefer(const QString& dbpath)
    {
        qDebug() << QSqlDatabase::drivers();

        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(dbpath);
        if (false == m_db.open())
        {
            qFatal(m_db.lastError().text().toStdString().c_str());
        }
    }

    ~DBDefer()
    {
        m_db.close();
    }

public:
    QSqlDatabase& db(void)
    {
        return m_db;
    }

private:
    QSqlDatabase m_db;
};

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    DBDefer dd("/Users/siwen/share/data.db");
    QSqlDatabase& db = dd.db();
    if (false == CommonStore::instance()->init(&db))
    {
        return -1;
    }
    if (false == GoodsNameStore::instance()->init(&db))
    {
        return -1;
    }
    if (false == ShopNameStore::instance()->init(&db))
    {
        return -1;
    }
    if (false == Store::instance()->init(&db))
    {
        return -1;
    }

    Dialog w;
    w.show();

    return a.exec();
}
