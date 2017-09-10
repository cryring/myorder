#ifndef GOODSNAMEMANAGEDIALOG_H
#define GOODSNAMEMANAGEDIALOG_H

#include <QDialog>

namespace Ui
{
class GoodsNameManageDialog;
}

class GoodsNameManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoodsNameManageDialog(QWidget* parent = 0);
    ~GoodsNameManageDialog();

private slots:
    void on_saveButton_clicked();

    void on_delButton_clicked();

private:
    void init(void);

private:
    Ui::GoodsNameManageDialog* ui;
};

#endif // GOODSNAMEMANAGEDIALOG_H
