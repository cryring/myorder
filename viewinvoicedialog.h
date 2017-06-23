#ifndef VIEWINVOICEDIALOG_H
#define VIEWINVOICEDIALOG_H

#include <QDialog>

namespace Ui {
class ViewInvoiceDialog;
}

class ViewInvoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewInvoiceDialog(QWidget *parent = 0);
    ~ViewInvoiceDialog();

private:
    Ui::ViewInvoiceDialog *ui;
};

#endif // VIEWINVOICEDIALOG_H
