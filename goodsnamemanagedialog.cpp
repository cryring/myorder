#include <QStandardItemModel>
#include "goodsnamemanagedialog.h"
#include "ui_goodsnamemanagedialog.h"
#include "goodsnamestore.h"

GoodsNameManageDialog::GoodsNameManageDialog(QWidget *parent) :
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

void GoodsNameManageDialog::init()
{
    QStandardItemModel* model = new QStandardItemModel(0,2);
    ui->goodsView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("品牌"));
    model->setHeaderData(col++, Qt::Horizontal, tr("名称"));


    int row = 0;
    const GNMAP& goodsNames = GoodsNameStore::instance()->getNames();
    GNMAP::const_iterator it = goodsNames.begin();
    for (; it != goodsNames.end(); ++it)
    {
        const QString& brand = it.key();
        const QVector<QString>& names = it.value();
        for (int i = 0; i < names.size(); ++i)
        {
            int col = 0;
            model->setItem(row, col++, new QStandardItem(brand));
            model->setItem(row, col++, new QStandardItem(names[i]));
            row++;
        }
    }
}
