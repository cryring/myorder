#ifndef NEWINVOICEDIALOG_H
#define NEWINVOICEDIALOG_H

#include <QDialog>
#include <QVector>

namespace Ui
{
class GoodsImportDialog;
}

struct GoodsAttribute;

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

private:
    void init();

    float calcTotalPrice(float paperTotalPrice);
    
    bool checkNeededInput(void);

    QString createID(const QString& date);

private:
    Ui::GoodsImportDialog* ui;

    QVector<GoodsAttribute*> m_goodsAttribute;

    QString m_date;
    QString m_invoiceId;
    QString m_payPrice;
    QString m_exchangeRate;
    QString m_coupon;
    QString m_couponDiscount;

};

#endif // NEWINVOICEDIALOG_H
