#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QUuid>
#include "goodsimportdialog.h"
#include "ui_goodsimportdialog.h"
#include "goodsdefine.h"
#include "shopnamestore.h"
#include "goodsnamestore.h"
#include "store.h"

GoodsImportDialog::GoodsImportDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::GoodsImportDialog)
{
    ui->setupUi(this);

    init();
}

GoodsImportDialog::~GoodsImportDialog()
{
    delete ui;

    for (int i = 0; i < m_goodsAttribute.size(); ++i)
    {
        delete m_goodsAttribute[i];
        m_goodsAttribute[i] = NULL;
    }
    m_goodsAttribute.clear();
}

void GoodsImportDialog::on_saveGoodsButton_clicked()
{
    QString count = ui->goodsCountEdit->text();
    if ("" == count)
    {
        // TODO: Message box
        QMessageBox::warning(this, tr("order"), tr("please input goods count."));
        return;
    }

    QString price = ui->goodsPriceEdit->text();
    if ("" == price)
    {
        // TODO: Message Box
        QMessageBox::warning(this, tr("order"), tr("please input goods price."));
        return;
    }

    QString attr = ui->attributeEdit->text();
    if ("" == attr)
    {
        // TODO: Message Box
        QMessageBox::warning(this, tr("order"), tr("please input goods attribute."));
        return;
    }

    QString name = ui->savedGoodsNameBox->currentText();

    QStandardItemModel* model = (QStandardItemModel*)ui->invoiceView->model();
    if (NULL == model)
    {
        // TODO: Message Box
        QMessageBox::warning(this, tr("order"), tr("wtf, view model is null."));
        return;
    }

    int col = 0;
    int row = model->rowCount();
    model->setItem(row, col++, new QStandardItem(name));
    model->setItem(row, col++, new QStandardItem(price));
    model->setItem(row, col++, new QStandardItem(attr));
    model->setItem(row, col++, new QStandardItem(count));

    GoodsAttribute* goodsAttr = new GoodsAttribute();
    goodsAttr->name = name;
    goodsAttr->price = price;
    goodsAttr->attribute = attr;
    goodsAttr->count = count;
    m_goodsAttribute.append(goodsAttr);
}

void GoodsImportDialog::on_delGoodsButton_clicked()
{
    int row = ui->invoiceView->currentIndex().row();
    if (0 <= row && m_goodsAttribute.size() > row)
    {
        m_goodsAttribute.remove(row);
    }
}

void GoodsImportDialog::on_shopNameBox_activated(const QString& name)
{
    qDebug() << "Combo:" << name;
}

void GoodsImportDialog::on_saveButton_clicked()
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
        QMessageBox::warning(this, tr("order"), tr("calc total price failed, please check the input."));
        return;
    }

    QString shopName = ui->shopNameBox->currentText();
    QString date = ui->dateEdit->date().toString("yyyyMMdd");
    QString invoiceid = createID(date);
    for (int i = 0; i < m_goodsAttribute.size(); ++i)
    {
        Goods goods;
        goods.id = createID(date);
        goods.invoiceid = invoiceid;
        goods.date = date;
        goods.shopName = shopName;
        goods.name = m_goodsAttribute[i]->name;
        goods.price = QString::number(m_goodsAttribute[i]->price.toFloat() / paperTotalPrice * totalPrice);
        goods.count = m_goodsAttribute[i]->count;
        goods.attribute = m_goodsAttribute[i]->attribute;
        Store::instance()->insertGoods(date, &goods);
    }
}

void GoodsImportDialog::init()
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
            auto brandGoodsNames = it.value();
            auto it = brandGoodsNames.begin();
            for (; it != brandGoodsNames.end(); it++)
            {
                ui->savedGoodsNameBox->addItem(*it);
            }
        }
    }

    QStandardItemModel* model = new QStandardItemModel(0, 4);
    ui->invoiceView->setModel(model);
    int col = 0;
    model->setHeaderData(col++, Qt::Horizontal, tr("商品名"));
    model->setHeaderData(col++, Qt::Horizontal, tr("价格"));
    model->setHeaderData(col++, Qt::Horizontal, tr("属性"));
    model->setHeaderData(col++, Qt::Horizontal, tr("数量"));
}

float GoodsImportDialog::calcTotalPrice(float paperTotalPrice)
{
    if (0 == paperTotalPrice)
    {
        return -1;
    }

    if (false == checkNeededInput())
    {
        return -1;
    }

    float fExchangeRate = m_exchangeRate.toFloat();
    float fCoupon = m_coupon.toFloat();
    float fCouponDiscount = m_couponDiscount.toFloat();
    float fRebate = m_rebate.toFloat();
    float fTaxFree = m_taxFree.toFloat();

    float p0 = fCoupon + paperTotalPrice;
    float p1 = p0 * (fRebate/100);
    float p2 = p0 * (fTaxFree/100);
    float p3 = fCoupon * ((100-fCouponDiscount)/100);
    float finish = p0 - p1 - p2 - p3;
    return finish / fExchangeRate;
}

bool GoodsImportDialog::checkNeededInput()
{
    m_payPrice = ui->payPriceEdit->text();
    m_exchangeRate = ui->exchangeRateEdit->text();
    m_coupon = ui->couponEdit->text();
    m_couponDiscount = ui->couponDiscountEdit->text();
    m_rebate = ui->rebateEdit->text();
    m_taxFree = ui->taxFreeEdit->text();

    if (m_payPrice.isEmpty() ||
        m_exchangeRate.isEmpty() ||
        m_coupon.isEmpty() ||
        m_couponDiscount.isEmpty() ||
        m_rebate.isEmpty() ||
        m_taxFree.isEmpty())
    {
        return false;
    }

    return true;
}

void GoodsImportDialog::on_brandComboBox_activated(const QString& brand)
{
    ui->savedGoodsNameBox->clear();
    const GNMAP& goodsNames = GoodsNameStore::instance()->getNames();
    if (goodsNames.contains(brand))
    {
        auto names = goodsNames[brand];
        auto it = names.begin();
        for (; it != names.end(); it++)
        {
            ui->savedGoodsNameBox->addItem(*it);
        }
    }
}

QString GoodsImportDialog::createID(const QString& date)
{
    QString uuid = QUuid::createUuid().toString();
    uuid.replace("{", "");
    uuid.replace("}", "");
    uuid.replace("-", "");
    return date + "_" + uuid;
}


