#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include "goodsnamemanagedialog.h"
#include "ui_goodsnamemanagedialog.h"
#include "goodsnamestore.h"

GoodsNameManageDialog::GoodsNameManageDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::GoodsNameManageDialog)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":icon/main.ico"));
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

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
        QMessageBox::warning(this, tr("order"), tr("please input brand."));
        return;
    }
    const QString& name = ui->nameEdit->text();
    if (name.isEmpty())
    {
        QMessageBox::warning(this, tr("order"), tr("please input name."));
        return;
    }
    const QString& id = ui->idEdit->text();
    if (id.isEmpty())
    {
        QMessageBox::warning(this, tr("order"), tr("wtf, the view model is null."));
        return;
    }

    GoodsNameStore::instance()->insert(id, brand, name);

    QStandardItemModel* model = (QStandardItemModel*)ui->goodsView->model();
    if (NULL == model)
    {
        QMessageBox::warning(this, tr("order"), tr("wtf, the view model is null."));
        return;
    }

    int col = 0;
    int row = model->rowCount();
    model->setItem(row, col++, new QStandardItem(brand));
    model->setItem(row, col++, new QStandardItem(name));
    model->setItem(row, col++, new QStandardItem(id));
}

void GoodsNameManageDialog::on_delButton_clicked()
{
    QAbstractItemModel* model = ui->goodsView->model();
    int row = ui->goodsView->currentIndex().row();
    if (0 > row || row >= model->rowCount())
    {
        qDebug() << "error order row";
        QMessageBox::warning(this, tr("order"), tr("please select one goods."));
        return;
    }

    QString brand = model->index(row, 0).data().toString();
    QString name = model->index(row, 1).data().toString();
    QString id = model->index(row, 2).data().toString();

    if (false != GoodsNameStore::instance()->remove(id))
    {
        model->removeRow(row);
    }
}

void GoodsNameManageDialog::init()
{
    QStandardItemModel* model = new QStandardItemModel(0, 3);
    ui->goodsView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("品牌"));
    model->setHeaderData(col++, Qt::Horizontal, tr("名称"));
    model->setHeaderData(col++, Qt::Horizontal, tr("代号"));


    int row = 0;
    const GNMAP& goodsNames = GoodsNameStore::instance()->getNames();
    GNMAP::const_iterator it = goodsNames.begin();
    for (; it != goodsNames.end(); ++it)
    {
        auto id = it.key();
        auto gn = it.value();

        int col = 0;
        model->setItem(row, col++, new QStandardItem(gn->brand));
        model->setItem(row, col++, new QStandardItem(gn->name));
        model->setItem(row, col++, new QStandardItem(id));
        row++;
    }
}
