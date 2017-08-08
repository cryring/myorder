#ifndef ORDERMANAGEDIALOG_H
#define ORDERMANAGEDIALOG_H

#include <QDialog>

namespace Ui
{
class OrderManageDialog;
}

class OrderManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderManageDialog(QWidget* parent = 0);
    ~OrderManageDialog();

private slots:
    void on_importTBorderButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::OrderManageDialog* ui;
};

#endif // ORDERMANAGEDIALOG_H
