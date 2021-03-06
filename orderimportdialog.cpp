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

    setWindowIcon(QIcon(":icon/main.ico"));
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

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
        QMessageBox::warning(this, tr("order"), tr("请检查你的输入文件，必须为同一日期."));
        return;
    }
    m_orderList.setDate(date);

    TaobaoExportOrderList tbOrderList;
    CsvLoader orderLoader(&tbOrderList);
    if (false == orderLoader.Load(orderFile))
    {
        QMessageBox::warning(this, tr("order"), tr("加载订单失败."));
        return;
    }

    TaobaoExportOrderDetailList tbOrderDetailList;
    CsvLoader orderDetailLoader(&tbOrderDetailList);
    if (false == orderDetailLoader.Load(orderDetailFile))
    {
        QMessageBox::warning(this, tr("order"), tr("加载订单详情失败."));
        return;
    }

    if (false == m_orderList.merge(&tbOrderList, &tbOrderDetailList))
    {
        QMessageBox::warning(this, tr("order"), tr("合并订单异常."));
        return;
    }

    QVector<QString> ids;
    m_orderList.getIds(ids);
    if (ids.empty())
    {
        QMessageBox::warning(this, tr("order"), tr("获取订单ID失败."));
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
        QMessageBox::warning(this, tr("order"), tr("订单列表为空"));
        return;
    }

    int size = ids.size();
    for (int i = 0; i < size; ++i)
    {
        Order* order = m_orderList.get(ids[i]);
        if (NULL == order)
        {
            qDebug() << "there is empty order in the list";
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
    setFixedSize(this->width(), this->height());
    ui->orderView->verticalHeader()->hide();
    // ui->orderView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->orderView->setSelectionBehavior(QAbstractItemView::SelectRows);

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
    ui->orderFileEdit->setText("/Users/siwen/Downloads/ExportOrderList201802121455.csv");
    ui->orderDetailFileEdit->setText("/Users/siwen/Downloads/ExportOrderDetailList201802121455.csv");
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

