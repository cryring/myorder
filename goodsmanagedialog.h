#ifndef INVOICEMANAGEDIALOG_H
#define INVOICEMANAGEDIALOG_H

#include <QDialog>

namespace Ui
{
class GoodsManageDialog;
}

class GoodsManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoodsManageDialog(QWidget* parent = 0);
    ~GoodsManageDialog();

private slots:
    void on_newInvoiceButton_clicked();

    void on_viewInvoiceButton_clicked();

private:
    Ui::GoodsManageDialog* ui;
};

#endif // INVOICEMANAGEDIALOG_H
