#ifndef VIEWINVOICEDIALOG_H
#define VIEWINVOICEDIALOG_H

#include <QDialog>

namespace Ui {
class ViewInvoiceDialog;
}

struct Goods;

class ViewInvoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewInvoiceDialog(QWidget *parent = 0);
    ~ViewInvoiceDialog();

private slots:
    void on_queryGoodsButton_clicked();

private:
    void init(void);

    void clearGoods(void);

private:
    Ui::ViewInvoiceDialog *ui;

    QVector<Goods*> m_curGoods;
};

#endif // VIEWINVOICEDIALOG_H
