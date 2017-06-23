#include <QDebug>
#include <QStandardItemModel>
#include <QFileDialog>
#include "orderimportdialog.h"
#include "ui_orderimportdialog.h"
#include "csvloader.h"
#include "taobaoexportorderdetaillist.h"
#include "taobaoexportorderlist.h"
#include "orderdetaillist.h"

#ifdef WINDOWS
    #define PATHSEP '\\'
#else
    #define PATHSEP '/'
#endif

OrderImportDialog::OrderImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderImportDialog)
{
    ui->setupUi(this);

    init();
}

OrderImportDialog::~OrderImportDialog()
{
    delete ui;
}

void OrderImportDialog::on_importOrderButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Order List"), " ", tr("CSV(*.csv)"));
    qDebug() << fileName;
    ui->orderFileEdit->setText(fileName);
}

void OrderImportDialog::on_importOrderDetailButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Order Detail List"), " ", tr("CSV(*.csv)"));
    qDebug() << fileName;
    ui->orderDetailFileEdit->setText(fileName);
}

void OrderImportDialog::on_createOrderButton_clicked()
{
    QString orderFile = ui->orderFileEdit->text();
    QString orderDetailFile = ui->orderDetailFileEdit->text();

    // Verify file name
    if (false == verifyOrderFile(orderFile, orderDetailFile))
    {
        // TODO: Message box
        qDebug() << "Error1";
        return;
    }

    TaobaoExportOrderList tbOrderList;
    TaobaoExportOrderDetailList tbOrderDetailList;
    CsvLoader orderLoader(&tbOrderList);
    CsvLoader orderDetailLoader(&tbOrderDetailList);
    if (false == orderLoader.Load(orderFile))
    {
        // TODO: Message box
        qDebug() << "Error2";
        return;
    }
    if (false == orderDetailLoader.Load(orderDetailFile))
    {
        // TODO: Message box
        qDebug() << "Error3";
        return;
    }

    OrderDetailList orderList;
    if (false == orderList.merge(&tbOrderList, &tbOrderDetailList))
    {
        // TODO: Message box
        qDebug() << "Error4";
        return;
    }

    QVector<QString> ids;
    orderList.getIds(ids);
    if (ids.empty())
    {
        // TODO:
        qDebug() << "Error5";
        return;
    }

    int row = 0;
    int size = ids.size();
    for (int i = 0; i < size; ++i)
    {
        QVector<OrderDetail*>* details = orderList.get(ids[i]);
        if (NULL == details)
        {
            // TODO:
            qDebug() << "Error6";
            return;
        }
        QStandardItemModel* model = (QStandardItemModel*)ui->orderView->model();
        if (NULL == model)
        {
            // TODO:
            qDebug() << "Error7";
            return;
        }

        for (int n = 0; n < details->size(); ++n)
        {
            int col = 0;
            model->setItem(row, col++, new QStandardItem(details->at(n)->id));
            model->setItem(row, col++, new QStandardItem(details->at(n)->title));
            model->setItem(row, col++, new QStandardItem(details->at(n)->price));
            model->setItem(row, col++, new QStandardItem(details->at(n)->postage));
            model->setItem(row, col++, new QStandardItem(details->at(n)->count));
            model->setItem(row, col++, new QStandardItem(details->at(n)->user_remark));
            model->setItem(row, col++, new QStandardItem(details->at(n)->sell_remark));
            model->setItem(row, col++, new QStandardItem(details->at(n)->user_name));
            model->setItem(row, col++, new QStandardItem(details->at(n)->user_alipay_id));
            row++;
        }
    }
}

void OrderImportDialog::on_saveButton_clicked()
{

}

void OrderImportDialog::init()
{
    QStandardItemModel* model = new QStandardItemModel(0,9);
    ui->orderView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("订单编号"));
    model->setHeaderData(col++, Qt::Horizontal, tr("标题"));
    model->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    model->setHeaderData(col++, Qt::Horizontal, tr("运费"));
    model->setHeaderData(col++, Qt::Horizontal, tr("数量"));
    model->setHeaderData(col++, Qt::Horizontal, tr("买家备注"));
    model->setHeaderData(col++, Qt::Horizontal, tr("卖家备注"));
    model->setHeaderData(col++, Qt::Horizontal, tr("用户名"));
    model->setHeaderData(col++, Qt::Horizontal, tr("支付宝ID"));

    // for test
    ui->orderFileEdit->setText("/Users/siwen/Downloads/ExportOrderList201706012311.csv");
    ui->orderDetailFileEdit->setText("/Users/siwen/Downloads/ExportOrderDetailList201706012311.csv");
}

bool OrderImportDialog::verifyOrderFile(QString orderFile, QString orderDetailFile)
{
    int pos = orderFile.lastIndexOf(PATHSEP);
    if (-1 == pos)
    {
        return false;
    }
    orderFile = orderFile.right(orderFile.length() - pos);
    qDebug() << orderFile;

    pos = orderDetailFile.lastIndexOf(PATHSEP);
    if (-1 == pos)
    {
        return false;
    }
    orderDetailFile = orderDetailFile.right(orderDetailFile.length() - pos);
    qDebug() << orderDetailFile;

    orderDetailFile.replace("Detail", "");
    if (orderDetailFile != orderFile)
    {
        return false;
    }

    return true;
}

