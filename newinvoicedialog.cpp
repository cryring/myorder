#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include "newinvoicedialog.h"
#include "ui_newinvoicedialog.h"
#include "goodsdefine.h"
#include "shopnamestore.h"
#include "goodsnamestore.h"
#include "store.h"

NewInvoiceDialog::NewInvoiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewInvoiceDialog)
{
    ui->setupUi(this);

    init();
}

NewInvoiceDialog::~NewInvoiceDialog()
{
    delete ui;

    for (int i = 0; i < m_goodsAttribute.size(); ++i)
    {
        delete m_goodsAttribute[i];
        m_goodsAttribute[i] = NULL;
    }
    m_goodsAttribute.clear();
}

void NewInvoiceDialog::on_saveGoodsButton_clicked()
{
    QString count = ui->goodsCountEdit->text();
    if ("" == count)
    {
        // TODO: Message box
        return;
    }

    QString price = ui->goodsPriceEdit->text();
    if ("" == price)
    {
        // TODO: Message Box
        return;
    }

    QString id = ui->idEdit->text();
    if ("" == id)
    {
        // TODO: Message Box
        return;
    }

    QString name = ui->savedGoodsNameBox->currentText();

    QStandardItemModel* model = (QStandardItemModel*)ui->invoiceView->model();
    if (NULL == model)
    {
        // TODO: Message Box
        return;
    }

    int col = 0;
    int row = model->rowCount();
    model->setItem(row, col++, new QStandardItem(name));
    model->setItem(row, col++, new QStandardItem(price));
    model->setItem(row, col++, new QStandardItem(count));

    GoodsAttribute* goodsAttr = new GoodsAttribute();
    goodsAttr->name = name;
    goodsAttr->price = price;
    goodsAttr->attribute = "";
    goodsAttr->count = count;
    m_goodsAttribute.append(goodsAttr);
}

void NewInvoiceDialog::on_delGoodsButton_clicked()
{
    int row = ui->invoiceView->currentIndex().row();
    if (0 <= row && m_goodsAttribute.size() > row)
    {
        m_goodsAttribute.remove(row);
    }
}

void NewInvoiceDialog::on_shopNameBox_activated(const QString &name)
{
    qDebug() << "Combo:" << name;
}

void NewInvoiceDialog::on_saveButton_clicked()
{
    float paperTotalPrice = 0;
    for (int i = 0; i < m_goodsAttribute.size(); ++i)
    {
        paperTotalPrice += m_goodsAttribute[i]->price.toFloat();
    }
    float totalPrice = calcTotalPrice(paperTotalPrice);
    if (totalPrice < 0)
    {
        // QMessageBox
        QMessageBox::warning(this, tr("OrderShark"), tr("Error Total Price"));
        return;
    }
    QString invoiceId = ui->idEdit->text();
    QString shopName = ui->shopNameBox->currentText();
    QString date = ui->dateEdit->date().toString("yyyyMMdd");
    for (int i = 0; i < m_goodsAttribute.size(); ++i)
    {
        Goods goods;
        goods.invoiceId = invoiceId;
        goods.shopName = shopName;
        goods.name = m_goodsAttribute[i]->name;
        goods.price = QString::number(m_goodsAttribute[i]->price.toFloat() / paperTotalPrice * totalPrice);
        goods.count = m_goodsAttribute[i]->count;
        goods.attribute = m_goodsAttribute[i]->attribute;
        Store::instance()->insertGoods(date, &goods);
    }
}

void NewInvoiceDialog::init()
{
    const QVector<QString>& shopNames = ShopNameStore::instance()->getNames();
    for (int i = 0; i < shopNames.size(); ++i)
    {
        ui->shopNameBox->addItem(shopNames[i]);
    }

    const GNMAP& goodsNames = GoodsNameStore::instance()->getNames();
    GNMAP::const_iterator it = goodsNames.begin();
    for (; it != goodsNames.end(); ++it)
    {
        ui->brandComboBox->addItem(it.key());
        if (0 == ui->savedGoodsNameBox->count())
        {
            const QVector<QString>& brandGoodsNames = it.value();
            for (int i = 0; i < brandGoodsNames.size(); ++i)
            {
                ui->savedGoodsNameBox->addItem(brandGoodsNames[i]);
            }
        }
    }

    ui->currencyBox->addItem("美元");
    ui->currencyBox->addItem("韩元");
    ui->currencyBox->addItem("人民币");

    QStandardItemModel* model = new QStandardItemModel(0,4);
    ui->invoiceView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("商品名"));
    model->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    model->setHeaderData(col++, Qt::Horizontal, tr("属性"));
    model->setHeaderData(col++, Qt::Horizontal, tr("数量"));
}

float NewInvoiceDialog::calcTotalPrice(float paperTotalPrice)
{
    if (0 == paperTotalPrice)
    {
        return -1;
    }

    QString payPrice = ui->payPriceEdit->text();
    QString feeRate = ui->feeRateEdit->text();
    QString disCount = ui->disCountEdit->text();
    QString exchangeRate = ui->exchangeRateEdit->text();
    QString coupon = ui->couponEdit->text();
    QString couponDiscount = ui->couponDiscountEdit->text();
    QString rebate = ui->rebateEdit->text();
    QString taxFree = ui->taxFreeEdit->text();

    if (payPrice.isEmpty() ||
        feeRate.isEmpty() ||
        exchangeRate.isEmpty() ||
        coupon.isEmpty() ||
        couponDiscount.isEmpty() ||
        rebate.isEmpty() ||
        taxFree.isEmpty())
    {
        return -1;
    }

    float fPayPrice = payPrice.toFloat();
    float fFeeRate = feeRate.toFloat();
    float fDiscount = disCount.toFloat();
    float fExchangeRate = exchangeRate.toFloat();
    float fCoupon = coupon.toFloat();
    float fCouponDiscount = couponDiscount.toFloat();
    float fRebate = rebate.toFloat();
    float fTaxFree = taxFree.toFloat();

    float p0 = paperTotalPrice * fRebate;
    float p1 = paperTotalPrice * fTaxFree;
    float p2 = fPayPrice * fDiscount * (1+fFeeRate);
    float p3 = fCoupon * fCouponDiscount;
    float p4 = (p2 + p3) - (p0 + p1);
    float p5 = p4 * fExchangeRate;
    return p5;
}


void NewInvoiceDialog::on_invoiceView_activated(const QModelIndex &index)
{
    qDebug() << "View actived" << index.row();
}

void NewInvoiceDialog::on_brandComboBox_activated(const QString &brand)
{
    ui->savedGoodsNameBox->clear();
    const GNMAP& goodsNames = GoodsNameStore::instance()->getNames();
    if (goodsNames.contains(brand))
    {
        const QVector<QString>& names = goodsNames[brand];
        for (int i = 0; i < names.size(); ++i)
        {
            ui->savedGoodsNameBox->addItem(names[i]);
        }
    }
}

