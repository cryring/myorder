#ifndef VIEWGOODSDIALOG_H
#define VIEWGOODSDIALOG_H

#include <QMap>
#include <QDialog>

namespace Ui {
class ViewGoodsDialog;
}

struct Goods;

class ViewGoodsDialog : public QDialog
{
    Q_OBJECT

public:
    enum
    {
        MODE_NORMAL,
        MODE_SETTLE,
    };

public:
    explicit ViewGoodsDialog(int mode = MODE_NORMAL, QWidget *parent = 0);

    ~ViewGoodsDialog();

    Goods* selectGoods(void);

private slots:
    void on_queryGoodsButton_clicked();

    void on_goodsView_doubleClicked(const QModelIndex &index);

private:
    void init(void);

    void clearGoods(void);

    void loadGoodsDate(void);

    void insertGoodsDate(const QString& date);

    void fillGoodsMonthBox(const QString& year);

    void fillGoodsDayBox(const QString& month);

private:
    Ui::ViewGoodsDialog *ui;

    int m_mode;

    int m_selectRow;

    QVector<Goods*> m_curGoods;

    QStringList m_tables;

    QMap<QString, QMap<QString, QStringList> > m_goodsDate;
};

#endif // VIEWGOODSDIALOG_H
