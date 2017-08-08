#ifndef VIEWORDERDIALOG_H
#define VIEWORDERDIALOG_H

#include <QDialog>

namespace Ui {
class ViewOrderDialog;
}

struct Order;

class ViewOrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewOrderDialog(QWidget *parent = 0);
    ~ViewOrderDialog();

private slots:
    void on_queryButton_clicked();

private:
    void init(void);

    void clearCurrentOrders(void);

private:
    Ui::ViewOrderDialog *ui;

    QVector<Order*> m_currentOrders;
};

#endif // VIEWORDERDIALOG_H
