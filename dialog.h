#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui
{
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget* parent = 0);
    ~Dialog();

private slots:
    void on_invoiceManageButton_clicked();

    void on_orderManageButton_clicked();

    void on_settleButton_clicked();

    void on_goodsNameManageButton_clicked();

    void on_shopManageButton_clicked();

private:
    void init(void);

private:
    Ui::Dialog* ui;
};

#endif // DIALOG_H
