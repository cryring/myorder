#ifndef GOODSSTORE_H
#define GOODSSTORE_H

#include <QString>
#include "goodsdefine.h"

class QSqlDatabase;

class GoodsStore
{
public:
    GoodsStore();

    ~GoodsStore();

public:
    bool init(QSqlDatabase* db);

    bool insert(Goods* goods);

    void select(const QString& date, QVector<Goods*>& goodss);

    void selectGoodsByInvoice(const QString& invoiceid, QVector<Goods*>& goodss);

    bool update(Goods* goods);

    bool remove(Goods* goods);

    bool removeByInvoiceID(const QString& date, const QString& invoiceid);

    bool exist(const QString& id);

    Goods* selectGoodsByID(const QString& id);

    bool updateSettle(const QString& id, bool settled);

private:
    QString createID(const QString& date);

private:
    QSqlDatabase* m_db;
};

#endif // GOODSSTORE_H
