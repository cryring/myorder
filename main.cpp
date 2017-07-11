#include <QApplication>
#include <QTextCodec>
#include <QtSql>
#include <QtDebug>
#include "dialog.h"
#include "store.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/siwen/share/data.db");
    if (false == db.open())
    {
        qDebug() << db.lastError();
        return false;
    }

    qDebug("db connected.");

    if (false == Store::instance()->init(&db))
    {
        return -1;
    }

    Dialog w;
    w.show();

    int ret = a.exec();
    db.close();
    return ret;
}
