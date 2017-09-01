#ifndef VIEWORDERDIALOG_H
#define VIEWORDERDIALOG_H

#include <QDialog>

namespace Ui
{
class ViewOrderDialog;
}

struct Order;

class ViewOrderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewOrderDialog(QWidget* parent = 0);
    ~ViewOrderDialog();

private slots:
    void on_queryButton_clicked();

    void on_queryAllButton_clicked();

    void on_allOrderView_activated(const QModelIndex &index);

    void on_allOrderView_clicked(const QModelIndex &index);

private:
    void init(void);

    void clearCurrentOrders(void);

    void queryOrder(const QString& date);

private:
    Ui::ViewOrderDialog* ui;

    QVector<Order*> m_currentOrders;
};

#endif // VIEWORDERDIALOG_H
