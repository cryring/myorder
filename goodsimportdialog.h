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

    GoodsImportDialog(const QString& invoiceid, const QString& date, QWidget* parent = 0);

    ~GoodsImportDialog();

    void setInvoiceID(const QString& id);

    void setDate(const QString& date);

private slots:
    void on_saveGoodsButton_clicked();

    void on_delGoodsButton_clicked();

    void on_saveButton_clicked();

    void on_calcButton_clicked();

    void on_goodsIdEdit_textChanged(const QString &id);

    void on_cancelButton_clicked();

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
    QString m_cash;
    QString m_cashRate;
    QString m_cashChange;
    QString m_coupon;
    QString m_couponDiscount;
    QString m_couponRate;
    QString m_creditCard;

};

#endif // NEWINVOICEDIALOG_H
