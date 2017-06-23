#ifndef NEWINVOICEDIALOG_H
#define NEWINVOICEDIALOG_H

#include <QDialog>

namespace Ui {
class NewInvoiceDialog;
}

class NewInvoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewInvoiceDialog(QWidget *parent = 0);

    ~NewInvoiceDialog();

private slots:
    void on_addGoodsButton_clicked();

    void on_editGoodsButton_clicked();

    void on_delGoodsButton_clicked();

    void on_shopNameBox_activated(const QString &arg1);

    void on_saveButton_clicked();

    void on_savedGoodsNameBox_activated(const QString &arg1);

private:
    void init();

private:
    Ui::NewInvoiceDialog *ui;
};

#endif // NEWINVOICEDIALOG_H
