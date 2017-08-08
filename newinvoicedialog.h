#ifndef NEWINVOICEDIALOG_H
#define NEWINVOICEDIALOG_H

#include <QDialog>
#include <QVector>

namespace Ui
{
class NewInvoiceDialog;
}

class GoodsAttribute;

class NewInvoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewInvoiceDialog(QWidget* parent = 0);

    ~NewInvoiceDialog();

private slots:
    void on_saveGoodsButton_clicked();

    void on_delGoodsButton_clicked();

    void on_shopNameBox_activated(const QString& arg1);

    void on_saveButton_clicked();

    void on_invoiceView_activated(const QModelIndex& index);

    void on_brandComboBox_activated(const QString& brand);

private:
    void init();

    float calcTotalPrice(float paperTotalPrice);

private:
    Ui::NewInvoiceDialog* ui;

    QVector<GoodsAttribute*> m_goodsAttribute;
};

#endif // NEWINVOICEDIALOG_H
