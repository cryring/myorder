#ifndef ORDERIMPORTDIALOG_H
#define ORDERIMPORTDIALOG_H

#include <QDialog>
#include <QVector>
#include "orderdetaillist.h"

namespace Ui {
class OrderImportDialog;
}

class OrderImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrderImportDialog(QWidget *parent = 0);
    ~OrderImportDialog();

private slots:
    void on_importOrderButton_clicked();

    void on_importOrderDetailButton_clicked();

    void on_createOrderButton_clicked();

    void on_saveButton_clicked();

private:
    void init();

    bool verifyOrderFile(QString orderFile, QString orderDetailFile, QString& date);

private:
    Ui::OrderImportDialog *ui;

    OrderDetailList m_orderList;
};

#endif // ORDERIMPORTDIALOG_H
