#ifndef ORDERIMPORTDIALOG_H
#define ORDERIMPORTDIALOG_H

#include <QDialog>

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

    bool verifyOrderFile(QString orderFile, QString orderDetailFile);

private:
    Ui::OrderImportDialog *ui;
};

#endif // ORDERIMPORTDIALOG_H
