#include <QtDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include "viewgoodsdialog.h"
#include "ui_viewgoodsdialog.h"
#include "store.h"
#include "goodsdefine.h"
#include "dboperation.h"
#include "goodsimportdialog.h"

ViewGoodsDialog::ViewGoodsDialog(int mode, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ViewGoodsDialog),
    m_mode(mode),
    m_selectRow(-1)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":icon/main.ico"));
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    init();
}

ViewGoodsDialog::~ViewGoodsDialog()
{
    clearGoods();
    delete ui;
}

Goods *ViewGoodsDialog::selectGoods()
{
    if (m_selectRow >= 0 && m_selectRow < m_curGoods.size())
    {
        return m_curGoods[m_selectRow]->clone();
    }
    return NULL;
}

void ViewGoodsDialog::on_queryGoodsButton_clicked()
{
    QString date = ui->yearBox->currentText() +
                   ui->monthBox->currentText() +
                   ui->dayBox->currentText();
    qDebug() << "goodsDateEdit:" << date;

    clearGoods();
    Store::instance()->getGoodsByDate(date, m_curGoods);

    if (m_curGoods.empty())
    {
        qDebug() << "empty goods";
        return;
    }

    QStandardItemModel* model = (QStandardItemModel*)ui->goodsView->model();
    if (NULL == model)
    {
        qDebug() << "empty model";
        return;
    }

    for (int i = 0; i < m_curGoods.size(); ++i)
    {
        Goods* goods = m_curGoods[i];
        int col = 0;
        model->setItem(i, col++, new QStandardItem(goods->settled ? "是" : "否"));
        model->setItem(i, col++, new QStandardItem(goods->name));
        model->setItem(i, col++, new QStandardItem(goods->price));
        model->setItem(i, col++, new QStandardItem(goods->attribute));
        model->setItem(i, col++, new QStandardItem(goods->count));
    }
}

void ViewGoodsDialog::init()
{
    QStandardItemModel* goodsModel = new QStandardItemModel(0, 5);
    ui->goodsView->setModel(goodsModel);
    int col = 0;
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("已结算"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("商品名"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("属性"));
    goodsModel->setHeaderData(col++, Qt::Horizontal, tr("数量"));

    ui->goodsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->goodsView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_tables = DBOperation::getAllTableName();
    loadGoodsDate();
}

void ViewGoodsDialog::clearGoods()
{
    int size = m_curGoods.size();
    for (int i = 0; i < size; ++i)
    {
        if (NULL != m_curGoods[i])
        {
            delete m_curGoods[i];
            m_curGoods[i] = NULL;
        }
    }
    m_curGoods.clear();
    ui->goodsView->reset();
}

void ViewGoodsDialog::loadGoodsDate()
{
    for (int i = 0; i < m_tables.size(); ++i)
    {
        const QString& tableName = m_tables[i];
        if (0 == tableName.indexOf("GOODS_20"))
        {
            QString date = tableName.right(8);
            insertGoodsDate(date);
        }
    }

    auto it = m_goodsDate.begin();
    for (; it != m_goodsDate.end(); it++)
    {
        ui->yearBox->addItem(it.key());
    }

    fillGoodsMonthBox(ui->yearBox->currentText());
    fillGoodsDayBox(ui->monthBox->currentText());
}

void ViewGoodsDialog::insertGoodsDate(const QString& date)
{
    QString year = date.left(4);
    QString day = date.right(2);
    QString month = date.mid(4, 2);
    m_goodsDate[year][month].append(day);
}

void ViewGoodsDialog::fillGoodsMonthBox(const QString& year)
{
    if (false == m_goodsDate.contains(year))
    {
        return;
    }

    auto months = m_goodsDate[year];
    auto it = months.begin();
    for (; it != months.end(); it++)
    {
        ui->monthBox->addItem(it.key());
    }
}

void ViewGoodsDialog::fillGoodsDayBox(const QString& month)
{
    auto year = ui->yearBox->currentText();
    if (false == m_goodsDate.contains(year))
    {
        return;
    }

    auto months = m_goodsDate[year];
    if (false == months.contains(month))
    {
        return;
    }

    auto days = months[month];
    for (int i = 0; i < days.size(); i++)
    {
        ui->dayBox->addItem(days[i]);
    }
}

void ViewGoodsDialog::on_goodsView_doubleClicked(const QModelIndex &index)
{
    if (m_mode == MODE_NORMAL)
    {
        return;
    }

    if (m_mode == MODE_SETTLE)
    {
        m_selectRow = index.row();
        if (m_selectRow >= 0 && m_selectRow < m_curGoods.size())
        {
            if (m_curGoods[m_selectRow]->settled)
            {
                QMessageBox::warning(this, tr("order"), tr("the goods has been settled."));
                m_selectRow = -1;
            }
            else
            {
                accept();
            }
        }
    }
}

void ViewGoodsDialog::on_openInvoiceButton_clicked()
{
    int row = ui->goodsView->currentIndex().row();
    if (row < 0 || row >= m_curGoods.size())
    {
        QMessageBox::warning(this, tr("order"), tr("please select one goods"));
        return;
    }
    QString invoiceid = m_curGoods[row]->invoiceid;
    QString date = m_curGoods[row]->date;

    GoodsImportDialog dlg;
    dlg.setInvoiceID(invoiceid);
    dlg.setDate(date);
    dlg.exec();
}
