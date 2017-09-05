#ifndef SETTLEDIALOG_H
#define SETTLEDIALOG_H

#include <QDialog>
#include <QVector>
#include <QMap>

namespace Ui
{
class SettleDialog;
}

struct Goods;
struct Order;

class SettleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettleDialog(QWidget* parent = 0);
    ~SettleDialog();

private slots:
    void on_settleButton_clicked();

    void on_exitButton_clicked();

    void on_attachButton_clicked();

    void on_detachButton_clicked();

    void on_orderSearchButton_clicked();

    void on_goodsSearchButton_clicked();

private:
    void init();

    void clearOrder(void);

    void clearGoods(void);

    void loadOrderDate(void);

    void loadGoodsDate(void);

    void getYMD(const QString& date, QString& year, QString& month, QString& day);

private:
    Ui::SettleDialog* ui;

    QVector<Goods*> m_curGoods;

    QVector<Order*> m_curOrder;

    QStringList m_tables;

    QMap<QString, QMap<QString, QString> > m_orderDate;

    QMap<QString, QMap<QString, QString> > m_goodsDate;
};

#endif // SETTLEDIALOG_H
