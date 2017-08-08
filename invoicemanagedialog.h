#ifndef INVOICEMANAGEDIALOG_H
#define INVOICEMANAGEDIALOG_H

#include <QDialog>

namespace Ui
{
class InvoiceManageDialog;
}

class InvoiceManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InvoiceManageDialog(QWidget* parent = 0);
    ~InvoiceManageDialog();

private slots:
    void on_newInvoiceButton_clicked();

    void on_viewInvoiceButton_clicked();

private:
    Ui::InvoiceManageDialog* ui;
};

#endif // INVOICEMANAGEDIALOG_H
