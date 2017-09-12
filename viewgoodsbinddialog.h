#ifndef VIEWGOODSBINDDIALOG_H
#define VIEWGOODSBINDDIALOG_H

#include <QDialog>

namespace Ui {
class ViewGoodsBindDialog;
}

struct Order;

class ViewGoodsBindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewGoodsBindDialog(QWidget *parent = 0);

    ~ViewGoodsBindDialog();

    void setOrder(Order* order);

    void load(void);

private slots:
    void on_detachButton_clicked();

private:
    void init(void);

private:
    Ui::ViewGoodsBindDialog *ui;

    Order* m_order;

    QStringList m_goodsID;
};

#endif // VIEWGOODSBINDDIALOG_H
