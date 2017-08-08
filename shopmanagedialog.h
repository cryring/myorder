#ifndef SHOPMANAGEDIALOG_H
#define SHOPMANAGEDIALOG_H

#include <QDialog>

namespace Ui {
class ShopManageDialog;
}

class ShopManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShopManageDialog(QWidget *parent = 0);
    ~ShopManageDialog();

private slots:
    void on_saveButton_clicked();

private:
    void init(void);

private:
    Ui::ShopManageDialog *ui;
};

#endif // SHOPMANAGEDIALOG_H
