#include <QDebug>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>
#include "orderimportdialog.h"
#include "ui_orderimportdialog.h"
#include "csvloader.h"
#include "taobaoexportorderdetaillist.h"
#include "taobaoexportorderlist.h"
#include "orderdetaillist.h"
#include "store.h"

#ifdef WINDOWS
#define PATHSEP '\\'
#else
#define PATHSEP '/'
#endif

OrderImportDialog::OrderImportDialog(QWidget* parent) :
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
    QString date;
    if (false == verifyOrderFile(orderFile, orderDetailFile, date))
    {
        // TODO: Message box
        QMessageBox::warning(this, tr("order"), tr("please check your input file, they must has the same date."));
        return;
    }
    m_orderList.setDate(date);

    TaobaoExportOrderList tbOrderList;
    TaobaoExportOrderDetailList tbOrderDetailList;
    CsvLoader orderLoader(&tbOrderList);
    CsvLoader orderDetailLoader(&tbOrderDetailList);
    if (false == orderLoader.Load(orderFile))
    {
        // TODO: Message box
        QMessageBox::warning(this, tr("order"), tr("load order file failed."));
        return;
    }
    if (false == orderDetailLoader.Load(orderDetailFile))
    {
        // TODO: Message box
        QMessageBox::warning(this, tr("order"), tr("load order detail file failed."));
        return;
    }

    if (false == m_orderList.merge(&tbOrderList, &tbOrderDetailList))
    {
        // TODO: Message box
        QMessageBox::warning(this, tr("order"), tr("merge two order file failed."));
        return;
    }

    QVector<QString> ids;
    m_orderList.getIds(ids);
    if (ids.empty())
    {
        // TODO:
        QMessageBox::warning(this, tr("order"), tr("get id from order list failed."));
        return;
    }

    int row = 0;
    int size = ids.size();
    for (int i = 0; i < size; ++i)
    {
        Order* details = m_orderList.get(ids[i]);
        if (NULL == details)
        {
            // TODO:
            qDebug() << "get details failed, " << ids[i];
            return;
        }
        QStandardItemModel* model = (QStandardItemModel*)ui->orderView->model();
        if (NULL == model)
        {
            // TODO:
            qDebug() << "Error7";
            return;
        }

        int col = 0;
        model->setItem(row, col++, new QStandardItem(details->id));
        model->setItem(row, col++, new QStandardItem(details->title));
        model->setItem(row, col++, new QStandardItem(details->price));
        model->setItem(row, col++, new QStandardItem(details->count));
        model->setItem(row, col++, new QStandardItem(details->user_remark));
        model->setItem(row, col++, new QStandardItem(details->sell_remark));
        model->setItem(row, col++, new QStandardItem(details->user_name));
        row++;
    }
}

void OrderImportDialog::on_saveButton_clicked()
{
    QString date = m_orderList.date();

    QVector<QString> ids;
    m_orderList.getIds(ids);
    if (ids.empty())
    {
        // TODO:
        qDebug() << "Error5";
        return;
    }

    int size = ids.size();
    for (int i = 0; i < size; ++i)
    {
        Order* order = m_orderList.get(ids[i]);
        if (NULL == order)
        {
            // TODO:
            qDebug() << "Error6";
            return;
        }

        if (false == Store::instance()->insertOrder(order))
        {
            return;
        }
    }
}

void OrderImportDialog::init()
{
    QStandardItemModel* model = new QStandardItemModel(0, 6);
    ui->orderView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("标题"));
    model->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    model->setHeaderData(col++, Qt::Horizontal, tr("数量"));
    model->setHeaderData(col++, Qt::Horizontal, tr("买家备注"));
    model->setHeaderData(col++, Qt::Horizontal, tr("卖家备注"));
    model->setHeaderData(col++, Qt::Horizontal, tr("用户名"));

    // for test
    ui->orderFileEdit->setText("/Users/siwen/Downloads/ExportOrderList201706012311.csv");
    ui->orderDetailFileEdit->setText("/Users/siwen/Downloads/ExportOrderDetailList201706012311.csv");
}

bool OrderImportDialog::verifyOrderFile(QString orderFile, QString orderDetailFile, QString& date)
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

    orderDetailFile.replace("/ExportOrderList", "");
    orderDetailFile.replace(".csv", "");
    qDebug() << "order date: " << orderDetailFile;
    if (12 != orderDetailFile.length())
    {
        return false;
    }
    date = orderDetailFile.left(8);
    return true;
}

