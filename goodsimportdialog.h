#ifndef NEWINVOICEDIALOG_H
#define NEWINVOICEDIALOG_H

#include <QDialog>
#include <QVector>

namespace Ui
{
class GoodsImportDialog;
}

struct Goods;

class GoodsImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoodsImportDialog(QWidget* parent = 0);

    ~GoodsImportDialog();

    void setInvoiceID(const QString& id);

    void setDate(const QString& date);

private slots:
    void on_saveGoodsButton_clicked();

    void on_delGoodsButton_clicked();

    void on_saveButton_clicked();

    void on_brandComboBox_activated(const QString& brand);

    void on_calcButton_clicked();

private:
    void init();

    float calcTotalPrice();
    
    bool checkNeededInput(void);

    QString createID(const QString& date);

private:
    Ui::GoodsImportDialog* ui;

    QVector<Goods*> m_goods;

    QString m_date;
    QString m_invoiceId;
    QString m_payPrice;
    QString m_exchangeRate;
    QString m_coupon;
    QString m_couponDiscount;
    QString m_cExchangeRate;
    QString m_creditCard;

};

#endif // NEWINVOICEDIALOG_H
