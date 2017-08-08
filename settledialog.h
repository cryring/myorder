#ifndef SETTLEDIALOG_H
#define SETTLEDIALOG_H

#include <QDialog>

namespace Ui {
class SettleDialog;
}

class SettleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettleDialog(QWidget *parent = 0);
    ~SettleDialog();

private slots:
    void on_settleButton_clicked();

    void on_exitButton_clicked();

    void on_attachButton_clicked();

    void on_detachButton_clicked();

private:
    Ui::SettleDialog *ui;
};

#endif // SETTLEDIALOG_H
