#include <QDebug>
#include <QStandardItemModel>
#include "goodsnamemanagedialog.h"
#include "ui_goodsnamemanagedialog.h"
#include "goodsnamestore.h"

GoodsNameManageDialog::GoodsNameManageDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::GoodsNameManageDialog)
{
    ui->setupUi(this);

    init();
}

GoodsNameManageDialog::~GoodsNameManageDialog()
{
    delete ui;
}

void GoodsNameManageDialog::on_saveButton_clicked()
{
    const QString& brand = ui->brandEdit->text();
    if (brand.isEmpty())
    {
        return;
    }
    const QString& name = ui->nameEdit->text();
    if (name.isEmpty())
    {
        return;
    }

    GoodsNameStore::instance()->insert(brand, name);

    QStandardItemModel* model = (QStandardItemModel*)ui->goodsView->model();
    if (NULL == model)
    {
        // TODO: Message Box
        return;
    }

    int col = 0;
    int row = model->rowCount();
    model->setItem(row, col++, new QStandardItem(brand));
    model->setItem(row, col++, new QStandardItem(name));
}

void GoodsNameManageDialog::on_delButton_clicked()
{
    QAbstractItemModel* model = ui->goodsView->model();
    int row = ui->goodsView->currentIndex().row();
    if (0 > row || row >= model->rowCount())
    {
        qDebug() << "error order row";
        return;
    }

    QString brand = model->index(row,0).data().toString();
    QString name = model->index(row,1).data().toString();

    if (false != GoodsNameStore::instance()->remove(brand, name))
    {
        model->removeRow(row);
    }
}

void GoodsNameManageDialog::init()
{
    QStandardItemModel* model = new QStandardItemModel(0, 2);
    ui->goodsView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("品牌"));
    model->setHeaderData(col++, Qt::Horizontal, tr("名称"));


    int row = 0;
    const GNMAP& goodsNames = GoodsNameStore::instance()->getNames();
    GNMAP::const_iterator it = goodsNames.begin();
    for (; it != goodsNames.end(); ++it)
    {
        auto brand = it.key();
        auto names = it.value();
        auto it = names.begin();
        for (; it != names.end(); it++)
        {
            int col = 0;
            model->setItem(row, col++, new QStandardItem(brand));
            model->setItem(row, col++, new QStandardItem(*it));
            row++;
        }
    }
}
