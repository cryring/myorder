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

private slots:
    void on_saveGoodsButton_clicked();

    void on_delGoodsButton_clicked();

    void on_shopNameBox_activated(const QString& arg1);

    void on_saveButton_clicked();

    void on_brandComboBox_activated(const QString& brand);

private:
    void init();

    float calcTotalPrice(float paperTotalPrice);
    
    bool checkNeededInput(void);

private:
    Ui::GoodsImportDialog* ui;

    QVector<GoodsAttribute*> m_goodsAttribute;

    QString m_payPrice;
    QString m_exchangeRate;
    QString m_coupon;
    QString m_couponDiscount;
    QString m_rebate;
    QString m_taxFree;
};

#endif // NEWINVOICEDIALOG_H
